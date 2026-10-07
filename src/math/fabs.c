// bdc 0x08a0c5e4 fabs
#include "bdc.h"

/* Ported from US legacy by byte match (unique masked-opcode hash): `fabs` at US `0x08a0c614`.
   Returns `x` with the sign bit (bit 31 of the high word) cleared. */
double fabs(double x)
{
    union { double d; struct { u32 lo; u32 hi; } w; } bits;

    bits.d = x;
    bits.w.hi &= 0x7fffffffU;
    return bits.d;
}
