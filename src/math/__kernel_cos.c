// bdc 0x08a0a7d4 __kernel_cos
#include "bdc.h"

/* fdlibm `__kernel_cos` (`k_cos.c`): cosine on `[-pi/4, pi/4]` of the reduced argument `x + y`
   (tail `y` from `__ieee754_rem_pio2`). `|x| < 2^-27` with `(int)x == 0` returns 1; otherwise
   the `C1..C6` polynomial of `g_kernelCosConsts`, with the `qx` correction for `|x| >= 0.3`
   (`qx` = `|x|/4` from the exponent bits, or 0.28125 above 0.78125). Called by `cos`. */
double __kernel_cos(double x, double y)
{
    const KernelCosConsts *k = &g_kernelCosConsts;
    union {
        double d;
        u32 words[2]; /* little-endian: words[0] low, words[1] high */
    } value, qx;
    s32 ix;
    double z;
    double r;
    double hz;
    double a;

    value.d = x;
    ix = (s32)(value.words[1] & 0x7fffffffu);
    if (ix < 0x3e400000 && (s32)x == 0) {
        return k->one; /* |x| < 2^-27, generate inexact */
    }
    z = x * x;
    r = z * (k->c1 + z * (k->c2 + z * (k->c3 + z * (k->c4 + z * (k->c5 + z * k->c6)))));
    if (ix < 0x3fd33333) { /* |x| < 0.3 */
        return k->one - (k->half * z - (z * r - x * y));
    }
    if (ix > 0x3fe90000) { /* |x| > 0.78125 */
        qx.d = k->qxBig;
    } else {
        qx.words[1] = (u32)(ix - 0x00200000); /* |x| / 4 */
        qx.words[0] = 0;
    }
    hz = k->half * z - qx.d;
    a = k->one - qx.d;
    return a - (hz - (z * r - x * y));
}
