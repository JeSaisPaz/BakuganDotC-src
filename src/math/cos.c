// bdc 0x08a047bc cos
#include "bdc.h"

/* Ported from US legacy by byte match (unique masked-opcode hash): `cos` at US `0x08a047ec`.
   fdlibm `cos`: |x| < pi/4 goes straight to `__kernel_cos`; inf/NaN return 0 (this build folds
   fdlibm's `x - x`); otherwise reduces with `__ieee754_rem_pio2` and picks +-`__kernel_cos` /
   +-`__kernel_sin` by quadrant. */
double cos(double x)
{
    union { double d; struct { u32 lo; u32 hi; } w; } bits;
    double y[2];
    u32 ix;

    bits.d = x;
    ix = bits.w.hi & 0x7fffffffU;
    if (ix < 0x3fe921fc) {
        return __kernel_cos(x, 0.0);
    }
    if (ix >= 0x7ff00000) {
        return 0.0;
    }
    switch (__ieee754_rem_pio2(x, y) & 3) {
    case 0:
        return __kernel_cos(y[0], y[1]);
    case 1:
        return -__kernel_sin(y[0], y[1], 1);
    case 2:
        return -__kernel_cos(y[0], y[1]);
    default:
        return __kernel_sin(y[0], y[1], 1);
    }
}
