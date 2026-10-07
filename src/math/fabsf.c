// bdc 0x08a0cb98 fabsf
#include "bdc.h"

/* Standard `fabsf`: clears the sign bit of `x` (works for -0 and NaN too). */
float fabsf(float x)
{
    union {
        float f;
        u32 bits;
    } value;

    value.f = x;
    value.bits &= 0x7fffffffu;
    return value.f;
}
