// bdc 0x08a0e768 __muldf3
#include "bdc.h"

/* libgcc soft-float `__muldf3` (`fp-bit.c`, inlined `_fpmul_parts`): unpacks both operands with
   `__unpack_d`; a NaN operand is returned (a first) with the product sign, `inf*0` / `0*inf`
   give the default NaN `__thenan_df`, an infinity or zero operand is returned with the product
   sign; otherwise the two 64-bit fractions are multiplied into a 128-bit high:low pair built from
   four 32x32 products, normalised into [IMPLICIT_1, IMPLICIT_2), guard-rounded (+0x80 when the
   guard byte is exactly 0x80 and the result is odd or `low` is non-zero) and packed with
   `__pack_d`. */

#define FRAC_IMPLICIT_1 0x1000000000000000ULL
#define FRAC_IMPLICIT_2 0x2000000000000000ULL
#define FRAC_HIGH 0x8000000000000000ULL

double __muldf3(double a, double b)
{
    FpNumber ua;
    FpNumber ub;
    FpNumber tmp;
    double av;
    double bv;
    FpNumber *res;
    u32 nl;
    u32 nh;
    u32 ml;
    u32 mh;
    u64 ppLl;
    u64 ppHl;
    u64 ppLh;
    u64 ppHh;
    u64 psHh;
    u64 high;
    u64 low;

    av = a;
    bv = b;
    __unpack_d(&av, &ua);
    __unpack_d(&bv, &ub);

    if (ua.fpClass < 2) {
        ua.sign = ua.sign != ub.sign;
        res = &ua;
    } else if (ub.fpClass < 2) {
        ub.sign = ua.sign != ub.sign;
        res = &ub;
    } else if (ua.fpClass == 4) {
        if (ub.fpClass == 2) {
            res = &__thenan_df;
        } else {
            ua.sign = ua.sign != ub.sign;
            res = &ua;
        }
    } else if (ub.fpClass == 4) {
        if (ua.fpClass == 2) {
            res = &__thenan_df;
        } else {
            ub.sign = ua.sign != ub.sign;
            res = &ub;
        }
    } else if (ua.fpClass == 2) {
        ua.sign = ua.sign != ub.sign;
        res = &ua;
    } else if (ub.fpClass == 2) {
        ub.sign = ua.sign != ub.sign;
        res = &ub;
    } else {
        /* 64x64 -> 128-bit multiply from four 32x32 products. */
        nl = (u32)ua.fraction;
        nh = (u32)(ua.fraction >> 32);
        ml = (u32)ub.fraction;
        mh = (u32)(ub.fraction >> 32);
        ppLl = (u64)ml * nl;
        ppHl = (u64)mh * nl;
        ppLh = (u64)ml * nh;
        ppHh = (u64)mh * nh;
        high = 0;
        psHh = ppHl + ppLh;
        if (psHh < ppHl) {
            high += 1ULL << 32;
        }
        ppHl = (u64)(u32)psHh << 32;
        low = ppLl + ppHl;
        if (low < ppLl) {
            high++;
        }
        high += (psHh >> 32) + ppHh;

        tmp.normalExp = ua.normalExp + ub.normalExp + 4;
        tmp.sign = ua.sign != ub.sign;
        while (high >= FRAC_IMPLICIT_2) {
            tmp.normalExp++;
            if (high & 1) {
                low >>= 1;
                low |= FRAC_HIGH;
            }
            high >>= 1;
        }
        while (high < FRAC_IMPLICIT_1) {
            tmp.normalExp--;
            high <<= 1;
            if (low & FRAC_HIGH) {
                high |= 1;
            }
            low <<= 1;
        }

        /* Guard byte exactly half way: round up if odd, or if `low` shows we are past half. */
        if ((high & 0xff) == 0x80) {
            if (high & 0x100) {
                high += 0x80;
            } else if (low != 0) {
                high += 0x80;
            }
        }
        tmp.fraction = high;
        tmp.fpClass = 3;
        res = &tmp;
    }
    return __pack_d(res);
}
