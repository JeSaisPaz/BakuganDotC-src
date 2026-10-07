// bdc 0x08a0a694 __ieee754_sqrtf
#include "bdc.h"

/* fdlibm/newlib `__ieee754_sqrtf`: bit-by-bit single-precision square root (integer only): inf/NaN
   give `x*x+x`, `±0` gives `x`, negatives NaN, subnormals normalised, then 25 result bits with the
   shift-and-subtract loop. Called by `__ieee754_acosf` and `__ieee754_powf`. */
float __ieee754_sqrtf(float x)
{
    union { float f; s32 i; } bits;
    s32 ix, exp, shift, sum, test, quot;
    u32 bit;

    bits.f = x;
    ix = bits.i;
    if ((ix & 0x7f800000) == 0x7f800000) {
        return x * x + x; /* sqrt(NaN) = NaN, sqrt(+inf) = +inf, sqrt(-inf) = NaN */
    }
    if (ix <= 0) {
        if ((ix & 0x7fffffff) == 0) {
            return x; /* sqrt(+-0) = +-0 */
        }
        if (ix < 0) {
            return 0.0f / 0.0f; /* sqrt(negative) = NaN */
        }
    }
    exp = ix >> 23;
    if (exp == 0) {
        /* subnormal: normalise */
        for (shift = 0; (ix & 0x800000) == 0; shift++) {
            ix <<= 1;
        }
        exp -= shift - 1;
    }
    exp -= 127;
    ix = (ix & 0x7fffff) | 0x800000;
    ix += ix;
    if (exp & 1) {
        ix += ix; /* odd exponent: double the mantissa */
    }
    exp >>= 1;

    quot = 0;
    sum = 0;
    for (bit = 0x1000000; bit != 0; bit >>= 1) {
        test = sum + (s32)bit;
        if (test <= ix) {
            sum = test + (s32)bit;
            ix -= test;
            quot += (s32)bit;
        }
        ix += ix;
    }
    if (ix != 0) {
        quot += quot & 1; /* round to nearest even */
    }
    bits.i = (quot >> 1) + 0x3f000000 + (exp << 23);
    return bits.f;
}
