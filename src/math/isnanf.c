// bdc 0x08a0ccc4 isnanf
#include "bdc.h"

/* Standard newlib `isnanf`: returns 1 when the magnitude bits of `x` exceed
 * the infinity pattern 0x7f800000 (a NaN), else 0. */
int isnanf(float x)
{
    union {
        float f;
        u32 bits;
    } value;

    value.f = x;
    return (int)((0x7f800000u - (value.bits & 0x7fffffffu)) >> 31);
}
