// bdc 0x08a0e344 _fpadd_parts
#include "bdc.h"

/* libgcc `fp-bit.c` static helper `_fpadd_parts`: adds two unpacked doubles (`FpNumber`; class
   0/1 = NaN, 2 = zero, 3 = normal, 4 = infinity). NaNs propagate, `inf + -inf` returns the default
   NaN at `0x08aa5000`, zeros combine signs, else aligns exponents and adds/subtracts fractions into
   `tmp`. Called by `__adddf3` and `__subdf3`. */

FpNumber *_fpadd_parts(FpNumber *a, FpNumber *b, FpNumber *tmp)
{
    int aExp;
    int bExp;
    u64 aFrac;
    u64 bFrac;
    s64 tFrac;
    int diff;

    if (a->fpClass < 2) {
        return a;
    }
    if (b->fpClass < 2) {
        return b;
    }
    if (a->fpClass == 4) {
        /* Adding infinities with opposite signs yields a NaN. */
        if (b->fpClass == 4 && a->sign != b->sign) {
            return &__thenan_df;
        }
        return a;
    }
    if (b->fpClass == 4) {
        return b;
    }
    if (b->fpClass == 2) {
        if (a->fpClass == 2) {
            *tmp = *a;
            tmp->sign = a->sign & b->sign;
            return tmp;
        }
        return a;
    }
    if (a->fpClass == 2) {
        return b;
    }

    aExp = a->normalExp;
    bExp = b->normalExp;
    aFrac = a->fraction;
    bFrac = b->fraction;

    diff = aExp - bExp;
    if (diff < 0) {
        diff = -diff;
    }
    if (diff < 64) {
        /* Shift the smaller operand right one bit at a time, keeping a sticky low bit. */
        if (aExp > bExp) {
            while (diff != 0) {
                bFrac = (bFrac & 1) | (bFrac >> 1);
                diff--;
            }
            bExp = aExp;
        }
        if (aExp < bExp) {
            while (diff != 0) {
                aFrac = (aFrac & 1) | (aFrac >> 1);
                diff--;
            }
            aExp = bExp;
        }
    } else if (aExp > bExp) {
        bFrac = 0;
    } else {
        aFrac = 0;
        aExp = bExp;
    }

    if (a->sign == b->sign) {
        tmp->sign = a->sign;
        tmp->normalExp = aExp;
        tmp->fraction = aFrac + bFrac;
    } else {
        if (a->sign != 0) {
            tFrac = (s64)(bFrac - aFrac);
        } else {
            tFrac = (s64)(aFrac - bFrac);
        }
        if (tFrac < 0) {
            tmp->sign = 1;
            tmp->normalExp = aExp;
            tmp->fraction = (u64)-tFrac;
        } else {
            tmp->normalExp = aExp;
            tmp->fraction = (u64)tFrac;
            tmp->sign = 0;
        }
        /* Normalise: shift left until the implicit bit (1 << 60) is set. */
        while (tmp->fraction - 1 < 0x0FFFFFFFFFFFFFFFULL) {
            tmp->normalExp--;
            tmp->fraction <<= 1;
        }
    }
    tmp->fpClass = 3;
    if (tmp->fraction >= 0x2000000000000000ULL) {
        tmp->fraction = (tmp->fraction & 1) | (tmp->fraction >> 1);
        tmp->normalExp++;
    }
    return tmp;
}
