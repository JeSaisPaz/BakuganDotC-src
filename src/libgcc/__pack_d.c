// bdc 0x08a0f0fc __pack_d
#include "bdc.h"

/* libgcc `fp-bit.c` `__pack_d`: converts an unpacked number (`FpNumber`) back to an IEEE double:
   NaN/inf/zero classes, denormal shifting for exponents below -1022 with round-to-nearest-even,
   overflow to infinity. Called by `__adddf3`, `__subdf3`, `__muldf3`, `__divdf3` and the
   other soft-float entry points. */

double __pack_d(const FpNumber *src)
{
    union {
        u64 bits;
        double value;
    } dst;
    u64 fraction = src->fraction;
    u32 sign = src->sign;
    s32 exp = 0;

    if (src->fpClass < 2) {
        /* signalling or quiet NaN: force the quiet bit */
        exp = 0x7ff;
        fraction |= 0x8000000000000ULL;
    } else if (src->fpClass == 4) {
        /* infinity */
        exp = 0x7ff;
        fraction = 0;
    } else if (src->fpClass == 2) {
        /* zero */
        fraction = 0;
    } else if (fraction != 0) {
        s32 normalExp = src->normalExp;

        if (normalExp < -0x3fe) {
            /* denormal: shift right, keeping a sticky bit */
            s32 shift = -0x3fe - normalExp;

            if (shift > 0x38) {
                fraction = 0;
            } else {
                u32 lowbit = (fraction & ((1ULL << shift) - 1)) != 0;
                fraction = (fraction >> shift) | lowbit;
            }
            if ((fraction & 0xff) == 0x80) {
                if ((fraction & 0x100) != 0) {
                    fraction += 0x80;
                }
            } else {
                fraction += 0x7f;
            }
            if (fraction >= (1ULL << 60)) {
                exp = 1;
            }
            fraction >>= 8;
        } else if (normalExp > 0x3ff) {
            /* overflow to infinity */
            exp = 0x7ff;
            fraction = 0;
        } else {
            exp = normalExp + 0x3ff;
            /* round to nearest, ties to even */
            if ((fraction & 0xff) == 0x80) {
                if ((fraction & 0x100) != 0) {
                    fraction += 0x80;
                }
            } else {
                fraction += 0x7f;
            }
            if (fraction >= (1ULL << 61)) {
                fraction >>= 1;
                exp += 1;
            }
            fraction >>= 8;
        }
    }

    dst.bits = (fraction & 0xfffffffffffffULL) | ((u64)((u32)exp & 0x7ff) << 52) |
               ((u64)sign << 63);
    return dst.value;
}
