"""Fill `.c.in` data templates from the game's module ELF (spec 2026-10-07-global-data-design.md D3,
D4, D10). Standard library only: the public release ships this file verbatim as `tools/fill_data.py`.

    python3 fill_data.py MYTHREAD-MAIN.elf [SRC_DIR] [OUT_DIR]     (defaults: src/data build/data)

A hole names an address in the ELF before relocation (0-based): `BDC_V(type, vaddr)`,
`BDC_BITS(storage, vaddr, bit, width)`, `BDC_STR(vaddr)`, `BDC_BYTES(vaddr, n)`. Every value is written so
that compiling it reproduces the ELF's bytes exactly."""
import hashlib
import math
import re
import struct
import sys
from pathlib import Path

ELF_SHA1 = "5ea4da7f8d25d18ceb1d437f0f95f42d92803ea0"
PT_LOAD = 1

# C scalar type -> (struct format, signed, literal suffix)
_SCALARS = {
    "char": ("b", True, ""), "signed char": ("b", True, ""), "unsigned char": ("B", False, ""),
    "_Bool": ("B", False, ""), "short": ("h", True, ""), "unsigned short": ("H", False, ""),
    "int": ("i", True, ""), "unsigned int": ("I", False, ""), "long": ("i", True, ""),
    "unsigned long": ("I", False, ""), "long long": ("q", True, "LL"),
    "unsigned long long": ("Q", False, "ULL"), "float": ("f", None, "f"), "double": ("d", None, ""),
}
_ADDR = r"(0x[0-9a-fA-F]+)"
_HOLE = re.compile(rf"BDC_V\(\s*([A-Za-z_][\w ]*?)\s*,\s*{_ADDR}\s*\)"
                   rf"|BDC_BITS\(\s*([A-Za-z_][\w ]*?)\s*,\s*{_ADDR}\s*,\s*(\d+)\s*,\s*(\d+)\s*\)"
                   rf"|BDC_STR\(\s*{_ADDR}\s*\)"
                   rf"|BDC_BYTES\(\s*{_ADDR}\s*,\s*(\d+)\s*\)")


class FillError(Exception):
    pass


def segments(elf: bytes) -> list[tuple[int, bytes]]:
    if elf[:4] != b"\x7fELF" or elf[4:6] != b"\x01\x01":
        raise FillError("not a 32-bit little-endian ELF")
    phoff, = struct.unpack_from("<I", elf, 0x1C)
    phentsize, phnum = struct.unpack_from("<HH", elf, 0x2A)
    out = []
    for i in range(phnum):
        p_type, p_offset, p_vaddr, _, p_filesz, *_ = struct.unpack_from("<8I", elf, phoff + i * phentsize)
        if p_type == PT_LOAD and p_filesz:
            out.append((p_vaddr, elf[p_offset:p_offset + p_filesz]))
    return out


def _read(segs, vaddr: int, n: int) -> bytes:
    for base, raw in segs:
        if base <= vaddr and vaddr + n <= base + len(raw):
            return raw[vaddr - base:vaddr - base + n]
    raise FillError(f"{vaddr:#x}+{n}: not in the ELF's initialised data")


def _cstring(segs, vaddr: int) -> bytes:
    for base, raw in segs:
        if base <= vaddr < base + len(raw):
            end = raw.find(b"\0", vaddr - base)
            if end < 0:
                raise FillError(f"{vaddr:#x}: string without a NUL")
            return raw[vaddr - base:end]
    raise FillError(f"{vaddr:#x}: not in the ELF's initialised data")


def _float(bits: int, width: int) -> str:
    exp_bits, man_bits = (8, 23) if width == 4 else (11, 52)
    sign = "-" if bits >> (width * 8 - 1) else ""
    exp = (bits >> man_bits) & ((1 << exp_bits) - 1)
    man = bits & ((1 << man_bits) - 1)
    f, d = ("f", "f") if width == 4 else ("", "")
    if exp == (1 << exp_bits) - 1:
        if man == 0:
            return f"{sign}__builtin_inf{f}()"
        quiet = 1 << (man_bits - 1)
        kind = "nan" if man & quiet else "nans"
        return f'{sign}__builtin_{kind}{f}("{man & ~quiet:#x}")'
    v = struct.unpack("<f" if width == 4 else "<d", bits.to_bytes(width, "little"))[0]
    text = float.hex(v)
    if math.copysign(1.0, v) < 0 and not text.startswith("-"):
        text = "-" + text
    text = re.sub(r"\.?0+p", "p", text) if "." in text else text  # 0x1.8000p+0 -> 0x1.8p+0
    text = text.replace("0xp", "0x0.0p").replace("0x0p", "0x0.0p")
    return text + d


def _scalar(ctype: str, raw: bytes) -> str:
    if ctype not in _SCALARS:
        raise FillError(f"unknown hole type {ctype}")
    fmt, signed, suffix = _SCALARS[ctype]
    if signed is None:
        return _float(int.from_bytes(raw, "little"), len(raw))
    u = int.from_bytes(raw, "little")
    if ctype == "_Bool":
        if u > 1:
            raise FillError(f"_Bool byte {u:#x} is neither 0 nor 1 (retype the field as u8)")
        return str(u)
    bits = len(raw) * 8
    if signed and u >> (bits - 1):
        # Decimal, not a cast hex literal: (long)0xffffffff is 4294967295 on LP64 hosts.
        v = u - (1 << bits)
        if v == -(1 << (bits - 1)):  # -2147483648 alone is a long/long long constant, not an int
            return f"({ctype})(-{-v - 1}{suffix} - 1)"
        return f"{v}{suffix}"
    return f"{u:#x}{suffix}"


def _bits(ctype: str, raw: bytes, lo: int, width: int) -> str:
    if ctype not in _SCALARS or _SCALARS[ctype][1] is None:
        raise FillError(f"unknown bit-field storage type {ctype}")
    v = (int.from_bytes(raw, "little") >> lo) & ((1 << width) - 1)
    if _SCALARS[ctype][1] and v >> (width - 1):
        return str(v - (1 << width))
    return f"{v:#x}"


def _string(b: bytes) -> str:
    out, hexed = [], False
    for c in b:
        ch = chr(c)
        if hexed and ch in "0123456789abcdefABCDEF":
            out.append('""')
        hexed = False
        if ch in '"\\?':
            out.append("\\" + ch)
        elif 0x20 <= c < 0x7F:
            out.append(ch)
        else:
            out.append(f"\\x{c:02x}")
            hexed = True
    return '"' + "".join(out) + '"'


def fill_text(text: str, segs) -> str:
    def one(m: re.Match) -> str:
        if m[1]:
            fmt = _SCALARS.get(m[1], (None,))[0]
            if fmt is None:
                raise FillError(f"unknown hole type {m[1]}")
            return _scalar(m[1], _read(segs, int(m[2], 16), struct.calcsize("<" + fmt)))
        if m[3]:
            lo, width = int(m[5]), int(m[6])
            return _bits(m[3], _read(segs, int(m[4], 16), (lo + width + 7) // 8), lo, width)
        if m[7]:
            return _string(_cstring(segs, int(m[7], 16)))
        raw = _read(segs, int(m[8], 16), int(m[9]))
        return "{ " + ", ".join(f"0x{b:02x}" for b in raw) + " }"
    out = _HOLE.sub(one, text)
    if "BDC_" in out:
        raise FillError(f"malformed hole: {out[out.index('BDC_'):][:60]}")
    return out


def fill_tree(src: Path, out: Path, elf: bytes, sha1: str | None = ELF_SHA1) -> list[Path]:
    got = hashlib.sha1(elf).hexdigest()
    if sha1 and got != sha1:
        raise FillError(f"ELF SHA-1 {got} is not the expected {sha1} (another region or revision)")
    segs = segments(elf)
    written = []
    for t in sorted(src.rglob("*.c.in")):
        dst = out / t.relative_to(src).with_suffix("")
        dst.parent.mkdir(parents=True, exist_ok=True)
        try:
            text = fill_text(t.read_text(encoding="utf-8"), segs)
        except FillError as e:
            raise FillError(f"{t}: {e}") from None
        if not dst.exists() or dst.read_text(encoding="utf-8") != text:
            dst.write_text(text, encoding="utf-8")
        written.append(dst)
    return written


def main(argv: list[str]) -> int:
    if not 2 <= len(argv) <= 4:
        print(__doc__.split("\n\n")[1].strip(), file=sys.stderr)
        return 2
    src = Path(argv[2] if len(argv) > 2 else "src/data")
    out = Path(argv[3] if len(argv) > 3 else "build/data")
    try:
        n = len(fill_tree(src, out, Path(argv[1]).read_bytes()))
    except (FillError, OSError) as e:
        print(f"fill_data: {e}", file=sys.stderr)
        return 1
    print(f"filled {n} templates into {out}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
