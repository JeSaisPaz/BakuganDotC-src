// bdc 0x08a0d18c copysignf
#include "bdc.h"

/* Ported from US legacy by byte match (unique masked-opcode hash): `copysignf` at US `0x08a0d1bc`.
   Returns the magnitude of `x` with the sign bit of `y`. */
float copysignf(float x, float y)
{
    union { float f; u32 u; } mag, sign;

    mag.f = x;
    sign.f = y;
    mag.u = (mag.u & 0x7fffffffU) | (sign.u & 0x80000000U);
    return mag.f;
}
