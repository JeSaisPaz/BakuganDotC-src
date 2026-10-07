// bdc 0x08a05ef8 rint
#include "bdc.h"

/* fdlibm `rint` (`s_rint.c`): rounds `x` to an integer in the current rounding mode by adding
   and subtracting `g_rintTwo52``[sign]` (2^52 with the sign of `x`), after sticky-bit fixups
   that keep the rounding of the dropped fraction bits correct. `|x| < 1` keeps the sign of `x`
   (so -0.4 gives -0); integral, infinite and NaN inputs return `x` (`x + x` for inf/NaN). */
double rint(double x)
{
    union {
        double d;
        u32 words[2]; /* little-endian: words[0] low, words[1] high */
    } value, w;
    s32 i0;
    s32 j0;
    s32 sx;
    u32 i;
    u32 i1;
    double t;

    value.d = x;
    i0 = (s32)value.words[1];
    i1 = value.words[0];
    sx = (i0 >> 31) & 1;
    j0 = ((i0 >> 20) & 0x7ff) - 0x3ff;

    if (j0 < 20) {
        if (j0 < 0) {
            if (((i0 & 0x7fffffff) | i1) == 0) {
                return x; /* +-0 */
            }
            i1 |= (u32)i0 & 0x0fffff;
            i0 &= (s32)0xfffe0000;
            i0 |= ((i1 | -i1) >> 12) & 0x80000;
            value.words[1] = (u32)i0;
            w.d = g_rintTwo52[sx] + value.d;
            w.d = w.d - g_rintTwo52[sx];
            w.words[1] = (w.words[1] & 0x7fffffffu) | ((u32)sx << 31);
            return w.d;
        }
        i = 0x000fffffu >> j0;
        if ((((u32)i0 & i) | i1) == 0) {
            return x; /* x is integral */
        }
        i >>= 1;
        if ((((u32)i0 & i) | i1) != 0) {
            if (j0 == 19) {
                i1 = 0x40000000;
            } else {
                i0 = (s32)(((u32)i0 & ~i) | (0x20000u >> j0));
            }
        }
    } else if (j0 > 51) {
        if (j0 == 0x400) {
            return x + x; /* inf or NaN */
        }
        return x; /* x is integral */
    } else {
        i = 0xffffffffu >> (j0 - 20);
        if ((i1 & i) == 0) {
            return x; /* x is integral */
        }
        i >>= 1;
        if ((i1 & i) != 0) {
            i1 = (i1 & ~i) | (0x40000000u >> (j0 - 20));
        }
    }
    value.words[1] = (u32)i0;
    value.words[0] = i1;
    t = g_rintTwo52[sx] + value.d;
    return t - g_rintTwo52[sx];
}
