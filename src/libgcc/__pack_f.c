// bdc 0x08a0f61c __pack_f
#include "bdc.h"

/* libgcc `fp-bit.c` `__pack_f`: single-precision version of `__pack_d`: NaN/inf/zero classes,
   denormals below exponent -126 with round-to-nearest-even, overflow to infinity; packs a 23-bit
   mantissa and 8-bit exponent. Called by `__make_fp`. */

float __pack_f(const FpNumberF *src)
{
    union {
        u32 bits;
        float value;
    } dst;
    u32 fraction = src->fraction;
    u32 sign = src->sign;
    s32 exp = 0;

    if (src->fpClass < 2) {
        /* signalling or quiet NaN: force the quiet bit */
        exp = 0xff;
        fraction |= 0x100000;
    } else if (src->fpClass == 4) {
        /* infinity */
        exp = 0xff;
        fraction = 0;
    } else if (src->fpClass == 2) {
        /* zero */
        fraction = 0;
    } else if (fraction != 0) {
        s32 normalExp = src->normalExp;

        if (normalExp < -0x7e) {
            /* denormal: shift right, keeping a sticky bit */
            s32 shift = -0x7e - normalExp;

            if (shift > 0x19) {
                fraction = 0;
            } else {
                u32 lowbit = (fraction & ((1u << shift) - 1)) != 0;
                fraction = (fraction >> shift) | lowbit;
            }
            if ((fraction & 0x7f) == 0x40) {
                if ((fraction & 0x80) != 0) {
                    fraction += 0x40;
                }
            } else {
                fraction += 0x3f;
            }
            if (fraction >= 0x40000000) {
                exp = 1;
            }
            fraction >>= 7;
        } else if (normalExp > 0x7f) {
            /* overflow to infinity */
            exp = 0xff;
            fraction = 0;
        } else {
            exp = normalExp + 0x7f;
            /* round to nearest, ties to even */
            if ((fraction & 0x7f) == 0x40) {
                if ((fraction & 0x80) != 0) {
                    fraction += 0x40;
                }
            } else {
                fraction += 0x3f;
            }
            if (fraction >= 0x80000000u) {
                fraction >>= 1;
                exp += 1;
            }
            fraction >>= 7;
        }
    }

    dst.bits = (fraction & 0x7fffff) | (((u32)exp & 0xff) << 23) | (sign << 31);
    return dst.value;
}
