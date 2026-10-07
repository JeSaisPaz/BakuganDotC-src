// bdc 0x08a0619c finitef
#include "bdc.h"

/* Ported from US legacy by byte match (unique masked-opcode hash): `finitef` at US `0x08a061cc`.
   Returns 1 when `x` is finite (magnitude bits below the infinity pattern 0x7f800000), else 0. */
int finitef(float x)
{
    union { float f; u32 u; } bits;

    bits.f = x;
    return (int)(((bits.u & 0x7fffffffU) - 0x7f800000U) >> 31);
}
