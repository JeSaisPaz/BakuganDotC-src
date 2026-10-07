// bdc 0x08a0c61c floor_libm
#include "bdc.h"

/* Second byte-identical copy of newlib `floor` (see `floor`): rounds `x` down to an integer
   (fdlibm `s_floor.c` bit-twiddling with the `huge + x > 0` inexact trick, `__cmpdf2` for the
   compare). This copy is the one linked into libm and is called only by `__kernel_rem_pio2`;
   the libc copy `floor` is used by `_fmt_double_digits`. */
double floor_libm(double x)
{
    union {
        double d;
        u32 words[2]; /* little-endian: words[0] low, words[1] high */
    } value;
    s32 i0;
    u32 i1;
    s32 j0;
    u32 mask;
    u32 j;

    value.d = x;
    i0 = (s32)value.words[1];
    i1 = value.words[0];
    j0 = ((i0 >> 20) & 0x7ff) - 0x3ff;

    if (j0 < 20) {
        if (j0 < 0) { /* |x| < 1 */
            if (g_floorLibmHuge + x > g_floorLibmZero) {
                if (i0 >= 0) {
                    i0 = 0;
                    i1 = 0;
                } else if (((i0 & 0x7fffffff) | i1) != 0) {
                    i0 = (s32)0xbff00000;
                    i1 = 0;
                }
            }
        } else {
            mask = 0x000fffffu >> j0;
            if (((i0 & mask) | i1) == 0) {
                return x; /* x is integral */
            }
            if (g_floorLibmHuge + x > g_floorLibmZero) {
                if (i0 < 0) {
                    i0 += 0x00100000 >> j0;
                }
                i0 &= ~mask;
                i1 = 0;
            }
        }
    } else if (j0 > 51) {
        if (j0 == 0x400) {
            return x + x; /* inf or NaN */
        }
        return x; /* x is integral */
    } else {
        mask = 0xffffffffu >> (j0 - 20);
        if ((i1 & mask) == 0) {
            return x; /* x is integral */
        }
        if (g_floorLibmHuge + x > g_floorLibmZero) {
            if (i0 < 0) {
                if (j0 == 20) {
                    i0 += 1;
                } else {
                    j = i1 + (1u << (52 - j0));
                    if (j < i1) {
                        i0 += 1; /* carry */
                    }
                    i1 = j;
                }
            }
            i1 &= ~mask;
        }
    }
    value.words[1] = (u32)i0;
    value.words[0] = i1;
    return value.d;
}
