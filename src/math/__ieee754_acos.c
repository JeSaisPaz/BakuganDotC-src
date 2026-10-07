// bdc 0x08a061bc __ieee754_acos
#include "bdc.h"

/* fdlibm/newlib `__ieee754_acos` (`e_acos.c`): arc cosine of `x` in double precision, the worker
   behind the `acos` wrapper (its only caller). `|x| == 1` returns 0 (x = 1) or pi (x = -1), other
   `|x| > 1` return NaN as `0.0 / 0.0`; `|x| <= 2^-57` returns `pio2Hi`; `|x| < 0.5` uses the
   `pS0..pS5`/`qS1..qS4` rational approximation `r`; larger `|x|` goes through `sqrt((1 -|x|) / 2)`.
   Constants from `g_acosConsts`. */
double __ieee754_acos(double x)
{
    const AcosConsts *k = &g_acosConsts;
    union {
        double d;
        u32 words[2]; /* little-endian: words[0] low, words[1] high */
    } value;
    double z;
    double p;
    double q;
    double r;
    double w;
    double s;
    double c;
    double df;
    s32 hx;
    s32 ix;

    value.d = x;
    hx = (s32)value.words[1];
    ix = hx & 0x7fffffff;
    if (ix >= 0x3ff00000) { /* |x| >= 1 */
        if (ix == 0x3ff00000 && value.words[0] == 0) { /* |x| == 1 */
            return hx >= 0 ? k->zero : k->pi;
        }
        return k->zero / k->zero; /* acos(|x| > 1) is NaN */
    }
    if (ix < 0x3fe00000) { /* |x| < 0.5 */
        if (ix <= 0x3c600000) {
            return k->pio2Hi; /* |x| < 2^-57 */
        }
        z = x * x;
        p = z * (k->pS0 + z * (k->pS1 + z * (k->pS2 + z * (k->pS3 + z * (k->pS4 + z * k->pS5)))));
        q = k->one + z * (k->qS1 + z * (k->qS2 + z * (k->qS3 + z * k->qS4)));
        r = p / q;
        return k->pio2Hi - (x - (k->pio2Lo - x * r));
    }
    if (hx < 0) { /* x < -0.5 */
        z = (k->one + x) * k->half;
        p = z * (k->pS0 + z * (k->pS1 + z * (k->pS2 + z * (k->pS3 + z * (k->pS4 + z * k->pS5)))));
        q = k->one + z * (k->qS1 + z * (k->qS2 + z * (k->qS3 + z * k->qS4)));
        s = __ieee754_sqrt(z);
        r = p / q;
        w = r * s - k->pio2Lo;
        return k->pi - k->two * (s + w);
    }
    /* x > 0.5 */
    z = (k->one - x) * k->half;
    s = __ieee754_sqrt(z);
    value.d = s;
    value.words[0] = 0;
    df = value.d;
    c = (z - df * df) / (s + df);
    p = z * (k->pS0 + z * (k->pS1 + z * (k->pS2 + z * (k->pS3 + z * (k->pS4 + z * k->pS5)))));
    q = k->one + z * (k->qS1 + z * (k->qS2 + z * (k->qS3 + z * k->qS4)));
    r = p / q;
    w = r * s + c;
    return k->two * (df + w);
}
