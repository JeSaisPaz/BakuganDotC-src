// bdc 0x08a0ed60 __floatsidf
#include "bdc.h"

/* libgcc soft-float `__floatsidf`: converts a 32-bit signed integer to double by building an
   `FpNumber` (class 3, exponent 60, fraction shifted until bit 60 is set) and packing it; `0`
   gives class zero and `INT_MIN` returns the constant `-2^31` (`g_floatsidfIntMin`) directly. */

double __floatsidf(int i)
{
    FpNumber n;

    n.sign = (u32)i >> 31;
    n.fpClass = 3;
    if (i == 0) {
        n.fpClass = 2;
    } else {
        s32 exp = 0x3c;
        n.normalExp = exp;
        if (n.sign != 0) {
            if (i == (s32)0x80000000) {
                return g_floatsidfIntMin;
            }
            i = -i;
        }
        n.fraction = (u64)(s64)i;
        while ((u32)(n.fraction >> 32) < 0x10000000) {
            n.fraction <<= 1;
            n.normalExp--;
        }
    }
    return __pack_d(&n);
}
