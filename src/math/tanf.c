// bdc 0x08a04924 tanf
#include "bdc.h"

/* Ported from US legacy by byte match (unique masked-opcode hash): `tanf` at US `0x08a04954`.
   fdlibm `tanf`: |x| <= pi/4 goes straight to `__kernel_tanf`; inf/NaN return 0 (this build
   folds fdlibm's `x - x` to a constant); otherwise reduces with `__ieee754_rem_pio2f` and calls
   `__kernel_tanf` with k = 1 (even quadrant) or -1 (odd). */
float tanf(float x)
{
    union { float f; s32 i; } bits;
    float y[2];
    s32 ix;
    int n;

    bits.f = x;
    ix = bits.i & 0x7fffffff;
    if (ix <= 0x3f490fda) {
        return __kernel_tanf(x, 0.0f, 1);
    }
    if (ix >= 0x7f800000) {
        return 0.0f;
    }
    n = __ieee754_rem_pio2f(x, y);
    return __kernel_tanf(y[0], y[1], 1 - ((n & 1) << 1));
}
