// bdc 0x089ba15c __ieee754_log
#include "bdc.h"

/* The fdlibm core `__ieee754_log(x)` (`e_log.c`): natural logarithm. Returns `-two54 / 0`
   (-Inf) for `x == 0`, `0 / 0` (NaN) for `x < 0`, `x + x` for infinity/NaN, and `0` for
   `x == 1`; otherwise reduces `x = 2^k * (1+f)` with `sqrt(2)/2 <= 1+f < sqrt(2)` and evaluates
   the `Lg1..Lg7` polynomial of `g_ieeeLogConsts`, adding `k*ln2_hi` and `k*ln2_lo`. */
double __ieee754_log(double x)
{
    const IeeeLogConsts *c = &g_ieeeLogConsts;
    const IeeeLogLiterals *lit = &g_ieeeLogLiterals;
    union {
        double d;
        u32 words[2]; /* little-endian: words[0] low, words[1] high */
    } value;
    s32 hx;
    s32 k;
    s32 i;
    s32 j;
    double f;
    double s;
    double z;
    double w;
    double t1;
    double t2;
    double r;
    double hfsq;
    double dk;

    value.d = x;
    hx = (s32)value.words[1];
    k = 0;
    if (hx < 0x00100000) { /* x < 2^-1022 */
        if (((hx & 0x7fffffff) | value.words[0]) == 0) {
            return lit->negTwo54 / c->zero; /* log(+-0) = -inf */
        }
        if (hx < 0) {
            return c->zero / c->zero; /* log(-#) = NaN */
        }
        k -= 54;
        value.d *= c->two54; /* scale up subnormal x */
        hx = (s32)value.words[1];
    }
    if (hx >= 0x7ff00000) {
        return value.d + value.d;
    }
    k += (hx >> 20) - 1023;
    hx &= 0x000fffff;
    i = (hx + 0x95f64) & 0x100000;
    value.words[1] = (u32)(hx | (i ^ 0x3ff00000)); /* normalise x or x/2 */
    k += i >> 20;
    f = value.d - g_ieeeLogOne;

    if ((0x000fffff & (2 + hx)) < 3) { /* -2^-20 <= f < 2^-20 */
        if (f == c->zero) {
            if (k == 0) {
                return c->zero;
            }
            dk = (double)k;
            return dk * c->ln2Hi + dk * c->ln2Lo;
        }
        r = f * f * (lit->half - lit->third * f);
        if (k == 0) {
            return f - r;
        }
        dk = (double)k;
        return dk * c->ln2Hi - ((r - dk * c->ln2Lo) - f);
    }

    s = f / (lit->two + f);
    dk = (double)k;
    z = s * s;
    i = hx - 0x6147a;
    w = z * z;
    j = 0x6b851 - hx;
    t1 = w * (c->lg2 + w * (c->lg4 + w * c->lg6));
    t2 = z * (c->lg1 + w * (c->lg3 + w * (c->lg5 + w * c->lg7)));
    i |= j;
    r = t1 + t2;
    if (i > 0) {
        hfsq = lit->half * f * f;
        if (k == 0) {
            return f - (hfsq - s * (hfsq + r));
        }
        return dk * c->ln2Hi - ((hfsq - (s * (hfsq + r) + dk * c->ln2Lo)) - f);
    }
    if (k == 0) {
        return f - s * (f - r);
    }
    return dk * c->ln2Hi - ((s * (f - r) - dk * c->ln2Lo) - f);
}
