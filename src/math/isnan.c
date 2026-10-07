// bdc 0x08a0c874 isnan
#include "bdc.h"

/* Ported from US legacy by byte match (unique masked-opcode hash): `isnan` at US `0x08a0c8a4`.
   Returns 1 when `x` is a NaN: the magnitude of the high word, with bit 0 set when any low-word
   mantissa bit is set, exceeds the infinity pattern 0x7ff00000. */
int isnan(double x)
{
    union { double d; struct { u32 lo; u32 hi; } w; } bits;
    u32 mag;

    bits.d = x;
    mag = (bits.w.hi & 0x7fffffffU) | ((bits.w.lo | -bits.w.lo) >> 31);
    return (int)((0x7ff00000U - mag) >> 31);
}
