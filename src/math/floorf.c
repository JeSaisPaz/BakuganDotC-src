// bdc 0x08a0cbb0 floorf
#include "bdc.h"

/* Ported from US legacy by byte match (unique masked-opcode hash): `floorf` at US `0x08a0cbe0`.
   fdlibm `floorf`: clears the fraction bits below the unbiased exponent, rounding negative
   non-integers down; |x| < 1 gives 0 / -1 (signed zero kept); inf/NaN return `x + x`. */
float floorf(float x)
{
    union { float f; s32 i; } bits;
    s32 exp;
    u32 fracMask;

    bits.f = x;
    exp = ((bits.i >> 23) & 0xff) - 0x7f;
    if (exp >= 23) {
        if (exp == 0x80) {
            return x + x; /* inf or NaN */
        }
        return x; /* already integral */
    }
    if (exp < 0) {
        /* |x| < 1; the huge-add only raises inexact */
        if (!(x + 1e30f <= 0.0f)) {
            if (bits.i >= 0) {
                bits.i = 0;
            } else if ((bits.i & 0x7fffffff) != 0) {
                bits.i = (s32)0xbf800000; /* -1.0f */
            }
        }
        return bits.f;
    }
    fracMask = 0x7fffffU >> exp;
    if (((u32)bits.i & fracMask) == 0) {
        return x; /* x is integral */
    }
    if (!(x + 1e30f <= 0.0f)) {
        if (bits.i < 0) {
            bits.i += 0x800000 >> exp;
        }
        bits.i &= ~fracMask;
    }
    return bits.f;
}
