#!/bin/sh
# Compile every C file under src/ to build/<target>/<subsystem>/<Name>.o. Compile-only: nothing links
# (see README.md). Usage: ./build.sh [mips|host]   (env: CLANG=clang, JOBS=<cpu count>)
set -eu
cd "$(dirname "$0")"

target=${1:-mips}
clang=${CLANG:-clang}
jobs=${JOBS:-$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 4)}

flags="-x c -std=gnu11 -ffreestanding -fno-math-errno -nostdlibinc -Isrc/include \
-Werror=implicit-function-declaration -Werror=implicit-int -Werror=int-conversion \
-Werror=incompatible-pointer-types -Werror=return-type"
case $target in
    mips) flags="$flags --target=mipsel-unknown-elf" ;;
    host) flags="$flags --target=x86_64-linux-gnu -ffp-contract=off \
-Werror=int-to-pointer-cast -Werror=pointer-to-int-cast" ;;
    *) echo "usage: $0 [mips|host]" >&2; exit 2 ;;
esac

out=build/$target
failed=$out/failed.txt
mkdir -p "$out"
: > "$failed"
export clang flags out failed
find src -name '*.c' | sort | xargs -P "$jobs" -n 16 sh -c '
    for c; do
        o=$out/${c#src/}; o=${o%.c}.o
        mkdir -p "${o%/*}"
        $clang $flags -c "$c" -o "$o" 2>"$o.log" || echo "$c" >> "$failed"
        [ -s "$o.log" ] || rm -f "$o.log"
    done' sh

total=$(find src -name '*.c' | wc -l)
bad=$(wc -l < "$failed")
warned=$(( $(find "$out" -name '*.o.log' | wc -l) - bad ))
echo "$target: $((total - bad))/$total compiled, $bad failed, $warned with warnings"
if [ "$bad" -ne 0 ]; then
    echo "failed files: $failed (errors in build/$target/<subsystem>/<Name>.o.log)" >&2
    exit 1
fi
