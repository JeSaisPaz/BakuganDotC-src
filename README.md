# BakuganDotC-src

Readable C decompilation of *Bakugan Battle Brawlers: Defenders of the Core* for the PSP (EU release,
disc `ULES01466`), main module `MYTHREAD-MAIN`.

No game files are included. See [LEGAL.md](LEGAL.md).

## Status

Every game function in the executable has a C version: 7,492 functions, one file each, all named,
typed, documented and reviewed. Every global the code uses is defined: zero-initialised and pointer-only definitions
are in `src/data/`, values are filled from your own ELF at build time.

| What it is | What it is not |
|---|---|
| Readable C with recovered names, structs and comments | A matching decompilation (the C does not rebuild the original binary byte for byte) |
| Every file compiles with clang, for MIPS (`mipsel-unknown-elf`) and x86-64 | A linked program: the build compiles objects only |
| A map of how the game works, function by function | A playable port: no platform layer, no asset loading |

### How it was made and checked

The code was exported from Ghidra, then AI agents cleaned it up and a separate agent reviewed each
function against the original MIPS disassembly. The cleanup allows struct fields only, no raw memory
offsets. Random re-audits by fresh reviewers re-checked 102 verified functions and failed 2 (about 2%).
Expect mistakes in names, comments and some logic. The code has not been checked by running it.

### What has no C body, by design

These are declared in `src/include/bdc.h` but not defined in `src/`:

- **213 PSP firmware imports** (`sce*`, `__sce*`). In the original executable each one is an 8-byte
  stub that the PSP kernel patches at load time to call the system software: they contain no game
  code. Their prototypes come from the PSPSDK and uOFW headers. 191 of them are called from the C.
  A port has to provide them, for example as an emulation layer.
- **`setjmp` / `longjmp`**: they save and restore CPU registers, which C cannot express. The host's
  C library provides them.
- **7 `Platform*` hooks** (data-cache writeback, FPU control register, VFPU random number generator)
  and **3 libm functions** (`sinf`, `cosf`, `asinf`) that the platform provides.

## Layout

```
src/include/bdc.h      every type, global (extern) and function prototype, plus the VFPU helpers
src/<subsystem>/*.c    one function per file; the first line is "// bdc <PSP address> <Name>"
src/data/<subsystem>/g_*.c     global definitions with no game value (zero or pointers): complete
src/data/<subsystem>/g_*.c.in  global definitions with value placeholders (templates), filled at build time
tools/fill_data.py     fills the templates from your ELF (Python 3, standard library only)
```

| Subsystem | Files | | Subsystem | Files |
|---|---:|---|---|---:|
| actor | 579 | | io | 128 |
| audio | 331 | | libc | 54 |
| battle | 1,403 | | libgcc | 22 |
| boot | 30 | | math | 57 |
| collision | 141 | | memory | 44 |
| core | 213 | | net | 220 |
| cxxrt | 61 | | render | 564 |
| game | 847 | | save | 102 |
| gmo | 409 | | script | 151 |
| input | 15 | | sysutil | 42 |
| ui | 2,079 | | | |

In comments, a name in backticks (`` `GfxMeshObjRunState` ``) is another function, global, type or field;
search `src/` for it.

PSP-specific vector-unit (VFPU) instructions are written as portable C helpers (`Vf*` in `bdc.h`) with
IEEE float results, not the VFPU's approximations. Build with `-ffp-contract=off` to keep them exact.

## Building (compile check)

Requires clang (tested with clang 19.1.7) and Python 3.

```sh
./build.sh mips path/to/MYTHREAD-MAIN.elf   # MIPS objects in build/mips/
./build.sh host path/to/MYTHREAD-MAIN.elf   # x86-64 objects in build/host/
```

The ELF is the game's main executable: decrypt `PSP_GAME/SYSDIR/EBOOT.BIN` from your own `ULES01466`
disc image with [pspdecrypt](https://github.com/John-K/pspdecrypt). Its SHA-1 is
`5ea4da7f8d25d18ceb1d437f0f95f42d92803ea0`. The build fills the data templates from it into `build/data/`
and compiles everything, printing `N/N compiled`. Without the ELF (`./build.sh mips`) the complete files
still compile and the build reports how many templates it skipped. Nothing links (see Status).

## Credits

- Started from [Vawlpe/BakuganDotC-decomp](https://github.com/Vawlpe/BakuganDotC-decomp) (GPL-3.0).
- PSP system function types and signatures:
  [PSPSDK](https://github.com/pspdev/pspsdk) (BSD-style, `LICENSE.pspsdk`) and
  [uOFW](https://github.com/uofw/uofw) (MIT, `LICENSE.uofw`).

## License

GPL-3.0 for this project's own work (`LICENSE`). The license does not cover the original game. See
[LEGAL.md](LEGAL.md).
