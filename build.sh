#!/bin/sh
# Compile every C file under src/ to build/<target>/<subsystem>/<Name>.o. Compile-only: nothing links
# (see README.md). With the ELF, the data templates (src/data/**/*.c.in) are first filled from it into
# build/data/ and compiled too.
# Usage: ./build.sh mips|host [path/to/MYTHREAD-MAIN.elf]   (env: CLANG=clang, JOBS=<cpu count>)
set -eu
cd "$(dirname "$0")"

target=${1:-mips}; elf=${2:-}
clang=${CLANG:-clang}
jobs=${JOBS:-$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 4)}

flags="-x c -std=gnu11 -ffreestanding -fno-math-errno -nostdlibinc -Isrc/include \
-Werror=implicit-function-declaration -Werror=implicit-int -Werror=int-conversion \
-Werror=incompatible-pointer-types -Werror=return-type"
case $target in
    mips) flags="$flags --target=mipsel-unknown-elf" ;;
    host) flags="$flags --target=x86_64-linux-gnu -ffp-contract=off \
-Werror=int-to-pointer-cast -Werror=pointer-to-int-cast" ;;
    *) echo "usage: $0 mips|host [path/to/MYTHREAD-MAIN.elf]" >&2; exit 2 ;;
esac

out=build/$target
failed=$out/failed.txt
mkdir -p "$out"
: > "$failed"
export clang flags out failed
rm -rf build/data
if [ -n "$elf" ]; then
    python3 tools/fill_data.py "$elf" src/data build/data || exit 1
fi
skipped=0
[ -n "$elf" ] || skipped=$(find src/data -name '*.c.in' | wc -l)
list=$(mktemp); { find src -name '*.c'; [ -d build/data ] && find build/data -name '*.c'; } | sort > "$list"
xargs -P "$jobs" -n 16 sh -c '
    for c; do
        rel=${c#src/}; rel=${rel#build/}
        o=$out/${rel%.c}.o
        mkdir -p "${o%/*}"
        $clang $flags -c "$c" -o "$o" 2>"$o.log" || echo "$c" >> "$failed"
        [ -s "$o.log" ] || rm -f "$o.log"
    done' sh < "$list"
total=$(wc -l < "$list"); rm -f "$list"
bad=$(wc -l < "$failed")
warned=$(( $(find "$out" -name '*.o.log' | wc -l) - bad ))
echo "$target: $((total - bad))/$total compiled, $bad failed, $warned with warnings"
[ "$skipped" -eq 0 ] || echo "data: $skipped templates not filled (pass the ELF to fill them)"
if [ "$bad" -ne 0 ]; then
    echo "failed files: $failed (errors in build/$target/<subsystem>/<Name>.o.log)" >&2
    exit 1
fi
