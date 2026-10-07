// bdc 0x089b9a98 floor
#include "bdc.h"

/* Ported from US legacy by byte match (same call site in a matched caller): `floor` at US
   `0x089b9ad8`. fdlibm `floor` (soft-float double): clears the fraction bits below the unbiased
   exponent, rounding negative non-integers down (with carry from the low into the high word);
   |x| < 1 gives +-0 / -1; inf/NaN return `x + x`. The `x + 1e300 > 0` tests only raise inexact. */
double floor(double x)
{
    union { double d; struct { u32 lo; u32 hi; } w; } bits;
    s32 hi, exp;
    u32 lo, mask, sum;

    bits.d = x;
    hi = (s32)bits.w.hi;
    lo = bits.w.lo;
    exp = ((hi >> 20) & 0x7ff) - 0x3ff;
    if (exp < 20) {
        if (exp < 0) {
            /* |x| < 1 */
            if (x + 1e300 > 0.0) {
                if (hi >= 0) {
                    hi = 0;
                    lo = 0;
                } else if (((hi & 0x7fffffff) | lo) != 0) {
                    hi = (s32)0xbff00000; /* -1.0 */
                    lo = 0;
                }
            }
        } else {
            mask = 0x000fffffU >> exp;
            if ((((u32)hi & mask) | lo) == 0) {
                return x; /* x is integral */
            }
            if (x + 1e300 > 0.0) {
                if (hi < 0) {
                    hi += 0x00100000 >> exp;
                }
                hi &= ~mask;
                lo = 0;
            }
        }
    } else if (exp > 51) {
        if (exp == 0x400) {
            return x + x; /* inf or NaN */
        }
        return x; /* x is integral */
    } else {
        mask = 0xffffffffU >> (exp - 20);
        if ((lo & mask) == 0) {
            return x; /* x is integral */
        }
        if (x + 1e300 > 0.0) {
            if (hi < 0) {
                if (exp == 20) {
                    hi += 1;
                } else {
                    sum = lo + (1U << (52 - exp));
                    if (sum < lo) {
                        hi += 1; /* carry */
                    }
                    lo = sum;
                }
            }
            lo &= ~mask;
        }
    }
    bits.w.hi = (u32)hi;
    bits.w.lo = lo;
    return bits.d;
}
