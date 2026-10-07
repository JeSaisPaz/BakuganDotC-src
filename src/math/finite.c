// bdc 0x08a05ec0 finite
#include "bdc.h"

/* fdlibm `finite(double x)`: returns 1 when `x` is neither infinite nor NaN, computed from the high
   word as `((hx & 0x7fffffff) - 0x7ff00000) >> 31`. */
int finite(double x)
{
    union {
        double d;
        u32 words[2]; /* little-endian: words[1] is the high word */
    } value;
    u32 hx;

    value.d = x;
    hx = value.words[1];
    return (int)(((hx & 0x7fffffffu) - 0x7ff00000u) >> 31);
}
