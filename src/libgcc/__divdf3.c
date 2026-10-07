// bdc 0x08a0eae0 __divdf3
#include "bdc.h"

/* libgcc soft-float `__divdf3` (`fp-bit.c`): unpacks both operands with `__unpack_d`, handles
   NaN/inf/zero classes (`inf/inf` and `0/0` give the default NaN at `0x08aa5000`, `x/0` gives
   infinity), else long-divides the 64-bit fractions bit by bit and packs with `__pack_d`. Called
   by libm for `(x-x)/(x-x)`-style NaN generation and divisions (`__ieee754_fmod`,
   `__ieee754_log`, `__ieee754_log10`, `pow`, `_fmt_double_digits`). */

double __divdf3(double a, double b)
{
    FpNumber ua;
    FpNumber ub;
    double av;
    double bv;
    FpNumber *res;
    u64 numerator;
    u64 denominator;
    u64 bit;
    u64 quotient;

    av = a;
    bv = b;
    __unpack_d(&av, &ua);
    __unpack_d(&bv, &ub);

    /* Inlined `_fpdiv_parts`. */
    if (ua.fpClass < 2) {
        res = &ua;
    } else if (ub.fpClass < 2) {
        res = &ub;
    } else {
        ua.sign = ua.sign ^ ub.sign;
        res = &ua;
        if (ua.fpClass == 4 || ua.fpClass == 2) {
            /* inf/inf and 0/0 are NaN. */
            if (ua.fpClass == ub.fpClass) {
                res = &__thenan_df;
            }
        } else if (ub.fpClass == 4) {
            ua.fraction = 0;
            ua.normalExp = 0;
        } else if (ub.fpClass == 2) {
            ua.fpClass = 4;
        } else {
            ua.normalExp = ua.normalExp - ub.normalExp;
            numerator = ua.fraction;
            denominator = ub.fraction;
            if (numerator < denominator) {
                numerator <<= 1;
                ua.normalExp--;
            }
            bit = 1ULL << 60;
            quotient = 0;
            while (bit != 0) {
                if (numerator >= denominator) {
                    quotient |= bit;
                    numerator -= denominator;
                }
                bit >>= 1;
                numerator <<= 1;
            }
            /* Round to nearest even on the 8 guard bits. */
            if ((quotient & 0xff) == 0x80) {
                if ((quotient & 0x100) != 0 || numerator != 0) {
                    quotient += 0x80;
                }
            }
            ua.fraction = quotient;
        }
    }
    return __pack_d(res);
}
