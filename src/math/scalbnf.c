// bdc 0x08a0cf80 scalbnf
#include "bdc.h"

/* Ported from US legacy by byte match (unique masked-opcode hash): `scalbnf` at US `0x08a0cfb0`.
   fdlibm `scalbnf`: returns `x * 2^n` by adjusting the exponent field, normalising subnormal
   inputs (`x * 2^25`) and producing signed overflow (`1e30 * 1e30`) or underflow
   (`1e-30 * 1e-30`) results via `copysignf`. */
float scalbnf(float x, int n)
{
    union { float f; s32 i; } bits;
    s32 exp;

    bits.f = x;
    exp = (bits.i & 0x7f800000) >> 23;
    if (exp == 0) {
        if ((bits.i & 0x7fffffff) == 0) {
            return x; /* +-0 */
        }
        x *= 33554432.0f; /* 2^25 */
        bits.f = x;
        exp = ((bits.i & 0x7f800000) >> 23) - 25;
        if (n < -50000) {
            return x * 1e-30f; /* underflow */
        }
    }
    if (exp == 0xff) {
        return x + x; /* inf or NaN */
    }
    exp += n;
    if (exp > 0xfe) {
        return copysignf(1e30f, x) * 1e30f; /* overflow */
    }
    if (exp > 0) {
        bits.i = (bits.i & (s32)0x807fffff) | (exp << 23);
        return bits.f;
    }
    if (exp <= -25) {
        if (n > 50000) {
            return copysignf(1e30f, x) * 1e30f; /* overflow in n + exp */
        }
        return copysignf(1e-30f, x) * 1e-30f; /* underflow */
    }
    exp += 25; /* subnormal result */
    bits.i = (bits.i & (s32)0x807fffff) | (exp << 23);
    return bits.f * 2.9802322e-08f; /* 2^-25 */
}
