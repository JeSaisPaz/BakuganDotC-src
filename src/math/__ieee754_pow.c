// bdc 0x08a06b48 __ieee754_pow
#include "bdc.h"

/* fdlibm/newlib `__ieee754_pow` (`e_pow.c`): `x` raised to `y` in double precision, the worker
   behind the `pow` wrapper (its only caller). Special cases first (`y == 0` gives 1, NaN
   operands give `x + y`, `y = +-Inf`, `+-1`, 2, 0.5, `x = 0/Inf/+-1`, negative `x` with non-integer
   `y` gives `0.0 / 0.0`), then `log2(|x|)` in extra precision (`t1 + t2`), times `y`, and `2^(y
   log2 x)` with overflow/underflow checks. Differences from the fdlibm source as compiled here:
   `(+-1) ** +-Inf` and the NaN results load 0.0 / compute `0.0 / 0.0`, the huge-`|y|` results
   are the folded constants +Inf / 0 without the sign `s`, and the `|x| ~ 1` path uses `x - 1`.
   Uses `fabs`, `scalbn`, `__ieee754_sqrt`; constants from `g_powConsts`. */
double __ieee754_pow(double x, double y)
{
    const PowConsts *k = &g_powConsts;
    union {
        double d;
        u32 words[2]; /* little-endian: words[0] low, words[1] high */
    } value;
    double z;
    double ax;
    double zH;
    double zL;
    double pH;
    double pL;
    double y1;
    double t1;
    double t2;
    double r;
    double s;
    double t;
    double u;
    double v;
    double w;
    double ss;
    double s2;
    double sH;
    double sL;
    double tH;
    double tL;
    s32 i;
    s32 j;
    s32 kk;
    s32 yisint;
    s32 n;
    s32 hx;
    s32 hy;
    s32 ix;
    s32 iy;
    u32 lx;
    u32 ly;

    value.d = x;
    hx = (s32)value.words[1];
    lx = value.words[0];
    value.d = y;
    hy = (s32)value.words[1];
    ly = value.words[0];
    ix = hx & 0x7fffffff;
    iy = hy & 0x7fffffff;

    /* y == zero: x**0 = 1 */
    if ((iy | ly) == 0) {
        return k->one;
    }

    /* +-NaN return x + y */
    if (ix > 0x7ff00000 || (ix == 0x7ff00000 && lx != 0) || iy > 0x7ff00000 ||
        (iy == 0x7ff00000 && ly != 0)) {
        return x + y;
    }

    /* determine if y is an odd int when x < 0:
       yisint = 0 ... y is not an integer, 1 ... odd integer, 2 ... even integer */
    yisint = 0;
    if (hx < 0) {
        if (iy >= 0x43400000) {
            yisint = 2; /* even integer y */
        } else if (iy >= 0x3ff00000) {
            kk = (iy >> 20) - 0x3ff; /* exponent */
            if (kk > 20) {
                j = (s32)(ly >> (52 - kk));
                if ((u32)(j << (52 - kk)) == ly) {
                    yisint = 2 - (j & 1);
                }
            } else if (ly == 0) {
                j = iy >> (20 - kk);
                if ((j << (20 - kk)) == iy) {
                    yisint = 2 - (j & 1);
                }
            }
        }
    }

    /* special value of y */
    if (ly == 0) {
        if (iy == 0x7ff00000) { /* y is +-inf */
            if (ix == 0x3ff00000 && lx == 0) {
                return k->zero; /* (+-1) ** +-inf */
            }
            if (ix >= 0x3ff00000) { /* (|x| > 1) ** +-inf = inf, 0 */
                return (hy >= 0) ? y : k->zero;
            }
            return (hy < 0) ? -y : k->zero; /* (|x| < 1) ** -,+inf = inf, 0 */
        }
        if (iy == 0x3ff00000) { /* y is +-1 */
            if (hy < 0) {
                return k->one / x;
            }
            return x;
        }
        if (hy == 0x40000000) {
            return x * x; /* y is 2 */
        }
        if (hy == 0x3fe00000 && hx >= 0) { /* y is 0.5, x >= +0 */
            return __ieee754_sqrt(x);
        }
    }

    ax = fabs(x);
    /* special value of x */
    if (lx == 0 && (ix == 0x7ff00000 || ix == 0 || ix == 0x3ff00000)) {
        z = ax; /* x is +-0, +-inf, +-1 */
        if (hy < 0) {
            z = k->one / z; /* z = (1 / |x|) */
        }
        if (hx < 0) {
            if (ix == 0x3ff00000 && yisint == 0) {
                z = k->zero / k->zero; /* (-1) ** non-int is NaN */
            } else if (yisint == 1) {
                z = -z; /* (x < 0) ** odd = -(|x| ** odd) */
            }
        }
        return z;
    }

    /* (x < 0) ** (non-int) is NaN */
    if (hx < 0 && yisint == 0) {
        return k->zero / k->zero;
    }

    /* |y| is huge */
    if (iy > 0x41e00000) { /* if |y| > 2^31 */
        if (iy > 0x43f00000) { /* if |y| > 2^64, must o/uflow */
            if (ix <= 0x3fefffff) {
                return (hy < 0) ? k->inf : k->zero;
            }
            if (ix >= 0x3ff00000) {
                return (hy >= 0) ? k->inf : k->zero;
            }
        }
        /* over/underflow if x is not close to one */
        if (ix < 0x3fefffff) {
            return (hy < 0) ? k->inf : k->zero;
        }
        if (ix > 0x3ff00000) {
            return (hy >= 0) ? k->inf : k->zero;
        }
        /* now |1 - x| is tiny <= 2^-20, suffice to compute log(x) by x - x^2/2 + x^3/3 - x^4/4 */
        t = x - k->one;
        w = (t * t) * (k->half - t * (k->third - t * k->quarter));
        u = k->ivln2H * t; /* ivln2H has 21 sig. bits */
        v = t * k->ivln2L - w * k->ivln2;
        t1 = u + v;
        value.d = t1;
        value.words[0] = 0;
        t1 = value.d;
        t2 = v - (t1 - u);
    } else {
        n = 0;
        /* take care of subnormal number */
        if (ix < 0x00100000) {
            ax *= k->two53;
            n -= 53;
            value.d = ax;
            ix = (s32)value.words[1];
        }
        n += (ix >> 20) - 0x3ff;
        j = ix & 0x000fffff;
        /* determine interval */
        ix = j | 0x3ff00000; /* normalize ix */
        if (j <= 0x3988e) {
            kk = 0; /* |x| < sqrt(3/2) */
        } else if (j < 0xbb67a) {
            kk = 1; /* |x| < sqrt(3) */
        } else {
            kk = 0;
            n += 1;
            ix -= 0x00100000;
        }
        value.d = ax;
        value.words[1] = (u32)ix;
        ax = value.d;

        /* compute ss = sH + sL = (x - 1) / (x + 1) or (x - 1.5) / (x + 1.5) */
        u = ax - k->bp[kk]; /* bp[0] = 1.0, bp[1] = 1.5 */
        v = k->one / (ax + k->bp[kk]);
        ss = u * v;
        value.d = ss;
        value.words[0] = 0;
        sH = value.d;
        /* tH = ax + bp[k] high */
        value.d = k->zero;
        value.words[1] = (u32)(((ix >> 1) | 0x20000000) + 0x00080000 + (kk << 18));
        tH = value.d;
        tL = ax - (tH - k->bp[kk]);
        sL = v * ((u - sH * tH) - sH * tL);
        /* compute log(ax) */
        s2 = ss * ss;
        r = s2 * s2 * (k->L1 + s2 * (k->L2 + s2 * (k->L3 + s2 * (k->L4 + s2 * (k->L5 + s2 * k->L6)))));
        r += sL * (sH + ss);
        s2 = sH * sH;
        tH = k->three + s2 + r;
        value.d = tH;
        value.words[0] = 0;
        tH = value.d;
        tL = r - ((tH - k->three) - s2);
        /* u + v = ss * (1 + ...) */
        u = sH * tH;
        v = sL * tH + tL * ss;
        /* 2 / (3 log2) * (ss + ...) */
        pH = u + v;
        value.d = pH;
        value.words[0] = 0;
        pH = value.d;
        pL = v - (pH - u);
        zH = k->cpH * pH; /* cpH + cpL = 2 / (3 * log2) */
        zL = k->cpL * pH + pL * k->cp + k->dpL[kk];
        /* log2(ax) = (ss + ...) * 2 / (3 * log2) = n + dpH + zH + zL */
        t = (double)n;
        t1 = ((zH + zL) + k->dpH[kk]) + t;
        value.d = t1;
        value.words[0] = 0;
        t1 = value.d;
        t2 = zL - (((t1 - t) - k->dpH[kk]) - zH);
    }

    s = k->one; /* s (sign of result -ve**odd) = -1 else = 1 */
    if (hx < 0 && yisint == 1) {
        s = k->negOne; /* (-ve) ** (odd int) */
    }

    /* split up y into y1 + y2 and compute (y1 + y2) * (t1 + t2) */
    value.d = y;
    value.words[0] = 0;
    y1 = value.d;
    pL = (y - y1) * t1 + y * t2;
    pH = y1 * t1;
    z = pL + pH;
    value.d = z;
    j = (s32)value.words[1];
    i = (s32)value.words[0];
    if (j >= 0x40900000) { /* z >= 1024 */
        if (((j - 0x40900000) | i) != 0) {
            return s * k->huge * k->huge; /* overflow */
        }
        if (pL + k->ovt > z - pH) {
            return s * k->huge * k->huge; /* overflow */
        }
    } else if ((j & 0x7fffffff) >= 0x4090cc00) { /* z <= -1075 */
        if (((u32)j - 0xc090cc00u | (u32)i) != 0) {
            return s * k->tiny * k->tiny; /* underflow */
        }
        if (pL <= z - pH) {
            return s * k->tiny * k->tiny; /* underflow */
        }
    }

    /* compute 2**(pH + pL) */
    i = j & 0x7fffffff;
    kk = (i >> 20) - 0x3ff;
    n = 0;
    if (i > 0x3fe00000) { /* if |z| > 0.5, set n = [z + 0.5] */
        n = j + (0x00100000 >> (kk + 1));
        kk = ((n & 0x7fffffff) >> 20) - 0x3ff; /* new k for n */
        value.d = k->zero;
        value.words[1] = (u32)(n & ~(0x000fffff >> kk));
        t = value.d;
        n = ((n & 0x000fffff) | 0x00100000) >> (20 - kk);
        if (j < 0) {
            n = -n;
        }
        pH -= t;
    }
    t = pL + pH;
    value.d = t;
    value.words[0] = 0;
    t = value.d;
    u = t * k->lg2H;
    v = (pL - (t - pH)) * k->lg2 + t * k->lg2L;
    z = u + v;
    w = v - (z - u);
    t = z * z;
    t1 = z - t * (k->P1 + t * (k->P2 + t * (k->P3 + t * (k->P4 + t * k->P5))));
    r = (z * t1) / (t1 - k->two) - (w + z * w);
    z = k->one - (r - z);
    value.d = z;
    j = (s32)value.words[1];
    j += (n << 20);
    if ((j >> 20) <= 0) {
        z = scalbn(z, n); /* subnormal output */
    } else {
        value.words[1] = (u32)j;
        z = value.d;
    }
    return s * z;
}
