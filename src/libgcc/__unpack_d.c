// bdc 0x08a0f39c __unpack_d
#include "bdc.h"

/* libgcc `fp-bit.c` `__unpack_d`: splits an IEEE double into an `FpNumber`: sign, class (2 zero,
   3 normal incl. normalised denormals with exponent -1022, 4 infinity, 0/1 signalling/quiet NaN by
   fraction bit 51), unbiased exponent and the fraction shifted left by 8 with the implicit bit at
   bit 60. Called by every soft-float double entry point. */

void __unpack_d(const double *src, FpNumber *dst)
{
    const u32 *words = (const u32 *)src;
    u32 hi = words[1];
    u32 lo = words[0];
    u32 exp = (hi >> 20) & 0x7ff;
    u64 fraction = ((u64)(hi & 0xfffff) << 32) | lo;

    dst->sign = hi >> 31;
    if (exp == 0) {
        if (fraction == 0) {
            dst->fpClass = 2; /* zero */
            return;
        }
        /* denormal: normalise so the leading bit sits at bit 60 */
        fraction <<= 8;
        dst->normalExp = -1022;
        dst->fpClass = 3;
        if (fraction < (1ULL << 60)) {
            int normalExp = -1022;
            do {
                fraction <<= 1;
                normalExp--;
            } while (fraction < (1ULL << 60));
            dst->normalExp = normalExp;
        }
    } else if (exp == 0x7ff) {
        if (fraction == 0) {
            dst->fpClass = 4; /* infinity */
            return;
        }
        /* NaN: quiet if fraction bit 51 is set; fraction kept unshifted */
        if ((hi & 0x80000) == 0) {
            dst->fpClass = 0;
        } else {
            dst->fpClass = 1;
        }
    } else {
        dst->fraction = (fraction << 8) | (1ULL << 60);
        dst->normalExp = (int)exp - 1023;
        dst->fpClass = 3;
        return;
    }
    dst->fraction = fraction;
}
