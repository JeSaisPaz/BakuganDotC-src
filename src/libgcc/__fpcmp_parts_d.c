// bdc 0x08a0f4b8 __fpcmp_parts_d
#include "bdc.h"

/* libgcc `fp-bit.c` `__fpcmp_parts_d`: three-way compare of two unpacked doubles: 1 if either is
   NaN, infinities and zeros by sign, else by sign, exponent and fraction; returns -1, 0 or 1.
   Called by `__cmpdf2`. */

int __fpcmp_parts_d(const FpNumber *a, const FpNumber *b)
{
    /* classes: 0/1 NaN, 2 zero, 3 normal, 4 infinity */
    if (a->fpClass < 2 || b->fpClass < 2) {
        return 1;
    }
    if (a->fpClass == 4 && b->fpClass == 4) {
        return (int)(b->sign - a->sign);
    }
    if (a->fpClass == 4) {
        return a->sign ? -1 : 1;
    }
    if (b->fpClass == 4) {
        return b->sign ? 1 : -1;
    }
    if (a->fpClass == 2 && b->fpClass == 2) {
        return 0;
    }
    if (a->fpClass == 2) {
        return b->sign ? 1 : -1;
    }
    if (b->fpClass == 2) {
        return a->sign ? -1 : 1;
    }
    if (a->sign != b->sign) {
        return a->sign ? -1 : 1;
    }
    if (a->normalExp > b->normalExp) {
        return a->sign ? -1 : 1;
    }
    if (a->normalExp < b->normalExp) {
        return a->sign ? 1 : -1;
    }
    if (a->fraction > b->fraction) {
        return a->sign ? -1 : 1;
    }
    if (a->fraction < b->fraction) {
        return a->sign ? 1 : -1;
    }
    return 0;
}
