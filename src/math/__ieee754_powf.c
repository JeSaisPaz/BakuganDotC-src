// bdc 0x08a0965c __ieee754_powf
#include "bdc.h"

/* fdlibm/newlib `__ieee754_powf` (`e_powf.c`): single-precision `x` raised to `y`, the worker
   behind `powf`. Same algorithm as `__ieee754_pow` in float: log2(|x|) as t1+t2 from the
   `g_powfConsts` `bp`/`dp_h`/`dp_l` tables, times y split in 12-bit halves, then 2**z by the
   exp polynomial and `scalbnf` for subnormal results; uses `fabsf`, `__ieee754_sqrtf`
   for y = 0.5 and `infinityf` for 0**negative. The binary differs from upstream where the
   compiler folded the special results: inf**+-1, (-1)**non-int and (x<0)**non-int return +0.0f
   (upstream NaN), every overflow returns +0.0f and the |y| > 2**27 shortcut returns 1.0f for
   "huge*huge" and 0.0f for "tiny*tiny"; that shortcut also takes log(x) of `x - 1`, not
   `|x| - 1`. Underflow keeps `s*tiny*tiny`. */

#define TWO24 0x1.0p+24f          /* 16777216.0, 0x4b800000 */
#define TINY 0x1.4484cp-100f      /* 1.0e-30, 0x0da24260 */
#define THIRD 0x1.555556p-2f      /* 0.333333343, 0x3eaaaaab */
#define L1 0x1.333334p-1f         /* 0.600000024, 0x3f19999a */
#define L2 0x1.b6db6ep-2f         /* 0.428571433, 0x3edb6db7 */
#define L3 0x1.555556p-2f         /* 0.333333343, 0x3eaaaaab */
#define L4 0x1.17460ap-2f         /* 0.272728115, 0x3e8ba305 */
#define L5 0x1.d864aap-3f         /* 0.230660751, 0x3e6c3255 */
#define L6 0x1.a7e284p-3f         /* 0.206975013, 0x3e53f142 */
#define P1 0x1.555556p-3f         /* 0.166666672, 0x3e2aaaab */
#define P2 -0x1.6c16c2p-9f        /* -0.00277777785, 0xbb360b61 */
#define P3 0x1.1566aap-14f        /* 6.61375598e-05, 0x388ab355 */
#define P4 -0x1.bbd41cp-20f       /* -1.6533902e-06, 0xb5ddea0e */
#define P5 0x1.637698p-25f        /* 4.13813694e-08, 0x3331bb4c */
#define LG2 0x1.62e43p-1f         /* 0.693147182, 0x3f317218 */
#define LG2_H 0x1.62e4p-1f        /* 0.693145752, 0x3f317200 */
#define LG2_L 0x1.7f7d18p-20f     /* 1.42860654e-06, 0x35bfbe8c */
#define OVT 0x1.715478p-25f       /* 4.29956657e-08, 0x3338aa3c */
#define CP 0x1.ec709ep-1f         /* 0.961796701, 0x3f76384f = 2/(3 ln2) */
#define CP_H 0x1.ec7p-1f          /* 0.961791992, 0x3f763800 head of CP */
#define CP_L 0x1.3b874p-18f       /* 4.70173836e-06, 0x369dc3a0 tail of CP_H */
#define IVLN2 0x1.715476p+0f      /* 1.44269502, 0x3fb8aa3b = 1/ln2 */
#define IVLN2_H 0x1.7154p+0f      /* 1.44268799, 0x3fb8aa00 16-bit head */
#define IVLN2_L 0x1.d94aep-18f    /* 7.05260754e-06, 0x36eca570 tail */

static s32 FloatBits(float f)
{
    union {
        float f;
        s32 i;
    } v;

    v.f = f;
    return v.i;
}

static float FloatFromBits(s32 i)
{
    union {
        float f;
        s32 i;
    } v;

    v.i = i;
    return v.f;
}

float __ieee754_powf(float x, float y)
{
    float z, ax, z_h, z_l, p_h, p_l;
    float y1, t1, t2, r, s, t, u, v, w;
    s32 i, j, k, yisint, n;
    s32 hx, hy, ix, iy;

    hx = FloatBits(x);
    hy = FloatBits(y);
    ix = hx & 0x7fffffff;
    iy = hy & 0x7fffffff;

    /* y == 0: x**0 = 1 */
    if (iy == 0) {
        return 1.0f;
    }
    /* x or y NaN */
    if (ix > 0x7f800000 || iy > 0x7f800000) {
        return x + y;
    }

    /* yisint: 0 = y not an integer, 1 = odd integer, 2 = even integer (only for x < 0) */
    yisint = 0;
    if (hx < 0) {
        if (iy >= 0x4b800000) {
            yisint = 2;
        } else if (iy >= 0x3f800000) {
            k = (iy >> 23) - 0x7f;
            j = iy >> (23 - k);
            if ((j << (23 - k)) == iy) {
                yisint = 2 - (j & 1);
            }
        }
    }

    /* y = +-inf */
    if (iy == 0x7f800000) {
        if (ix == 0x3f800000) {
            return 0.0f; /* upstream y - y (NaN), folded */
        }
        if (ix > 0x3f800000) { /* (|x| > 1) ** +-inf = inf, 0 */
            return (hy >= 0) ? y : 0.0f;
        }
        return (hy < 0) ? -y : 0.0f; /* (|x| < 1) ** -,+inf = inf, 0 */
    }
    if (iy == 0x3f800000) { /* y = +-1 */
        if (hy < 0) {
            return 1.0f / x;
        }
        return x;
    }
    if (hy == 0x40000000) { /* y = 2 */
        return x * x;
    }
    if (hy == 0x3f000000 && hx >= 0) { /* y = 0.5, x >= +0 */
        return __ieee754_sqrtf(x);
    }

    ax = fabsf(x);
    /* x = +-0, +-inf, +-1 */
    if (ix == 0x7f800000 || ix == 0 || ix == 0x3f800000) {
        z = ax;
        if (hy < 0) {
            if (ax == 0.0f) {
                return infinityf();
            }
            z = 1.0f / z;
        }
        if (hx < 0) {
            if (((ix - 0x3f800000) | yisint) == 0) {
                z = 0.0f; /* (-1) ** non-int: upstream (z-z)/(z-z) (NaN), folded */
            } else if (yisint == 1) {
                z = -z; /* (x < 0) ** odd = -(|x| ** odd) */
            }
        }
        return z;
    }

    /* (x < 0) ** non-int: upstream (x-x)/(x-x) (NaN), folded */
    if (((((u32)hx >> 31) - 1) | yisint) == 0) {
        return 0.0f;
    }

    if (iy > 0x4d000000) { /* |y| > 2**27 */
        /* over/underflow if x is not close to one */
        if (ix < 0x3f7ffff8) {
            return (hy < 0) ? 1.0f : 0.0f; /* upstream huge*huge : tiny*tiny, folded */
        }
        if (ix > 0x3f800007) {
            return (hy > 0) ? 1.0f : 0.0f; /* upstream huge*huge : tiny*tiny, folded */
        }
        /* |1 - x| is tiny: log(x) by x - x^2/2 + x^3/3 - x^4/4 */
        t = x - 1.0f;
        w = (t * t) * (0.5f - t * (THIRD - t * 0.25f));
        u = IVLN2_H * t;
        v = t * IVLN2_L - w * IVLN2;
        t1 = FloatFromBits(FloatBits(u + v) & 0xfffff000);
        t2 = v - (t1 - u);
    } else {
        float s2, s_h, s_l, t_h, t_l;

        n = 0;
        /* subnormal x */
        if (ix < 0x00800000) {
            ax *= TWO24;
            n -= 24;
            ix = FloatBits(ax);
        }
        n += (ix >> 23) - 0x7f;
        j = ix & 0x007fffff;
        /* determine interval */
        ix = j | 0x3f800000;
        if (j <= 0x1cc471) { /* |x| < sqrt(3/2) */
            k = 0;
        } else if (j < 0x5db3d7) { /* |x| < sqrt(3) */
            k = 1;
        } else {
            k = 0;
            n += 1;
            ix -= 0x00800000;
        }
        ax = FloatFromBits(ix);

        /* s = s_h + s_l = (x - 1)/(x + 1) or (x - 1.5)/(x + 1.5) */
        u = ax - g_powfConsts.bp[k];
        v = 1.0f / (ax + g_powfConsts.bp[k]);
        s = u * v;
        s_h = FloatFromBits(FloatBits(s) & 0xfffff000);
        /* t_h = ax + bp[k], high part */
        t_h = FloatFromBits(((ix >> 1) | 0x20000000) + 0x0040000 + (k << 21));
        t_l = ax - (t_h - g_powfConsts.bp[k]);
        s_l = v * ((u - s_h * t_h) - s_h * t_l);
        /* log(ax) */
        s2 = s * s;
        r = s2 * s2 * (L1 + s2 * (L2 + s2 * (L3 + s2 * (L4 + s2 * (L5 + s2 * L6)))));
        r += s_l * (s_h + s);
        s2 = s_h * s_h;
        t_h = FloatFromBits(FloatBits(3.0f + s2 + r) & 0xfffff000);
        t_l = r - ((t_h - 3.0f) - s2);
        /* u + v = s*(1 + ...) */
        u = s_h * t_h;
        v = s_l * t_h + t_l * s;
        /* 2/(3 log2) * (s + ...) */
        p_h = FloatFromBits(FloatBits(u + v) & 0xfffff000);
        p_l = v - (p_h - u);
        z_h = CP_H * p_h;
        z_l = CP_L * p_h + p_l * CP + g_powfConsts.dpL[k];
        /* log2(ax) = (s + ...)*2/(3 log2) = n + dp_h + z_h + z_l */
        t = (float)n;
        t1 = FloatFromBits(FloatBits(((z_h + z_l) + g_powfConsts.dpH[k]) + t) & 0xfffff000);
        t2 = z_l - (((t1 - t) - g_powfConsts.dpH[k]) - z_h);
    }

    /* sign of the result: -1 for (negative) ** (odd int) */
    s = 1.0f;
    if (((((u32)hx >> 31) - 1) | (yisint - 1)) == 0) {
        s = -1.0f;
    }

    /* split y into y1 + y2 and compute (y1 + y2)*(t1 + t2) */
    y1 = FloatFromBits(FloatBits(y) & 0xfffff000);
    p_l = (y - y1) * t1 + y * t2;
    p_h = y1 * t1;
    z = p_l + p_h;
    j = FloatBits(z);
    if (j > 0x43000000) {
        return 0.0f; /* overflow: upstream s*huge*huge, folded */
    }
    if (j == 0x43000000) {
        if (!(p_l + OVT <= z - p_h)) {
            return 0.0f; /* overflow: upstream s*huge*huge, folded */
        }
    } else if ((j & 0x7fffffff) > 0x43160000) {
        return s * TINY * TINY; /* underflow */
    } else if (j == (s32)0xc3160000) {
        if (p_l <= z - p_h) {
            return s * TINY * TINY; /* underflow */
        }
    }

    /* 2**(p_h + p_l) */
    i = j & 0x7fffffff;
    k = (i >> 23) - 0x7f;
    n = 0;
    if (i > 0x3f000000) { /* |z| > 0.5: n = [z + 0.5] */
        n = j + (0x00800000 >> (k + 1));
        k = ((n & 0x7fffffff) >> 23) - 0x7f; /* new k for n */
        t = FloatFromBits(n & ~(0x007fffff >> k));
        n = ((n & 0x007fffff) | 0x00800000) >> (23 - k);
        if (j < 0) {
            n = -n;
        }
        p_h -= t;
    }
    t = FloatFromBits(FloatBits(p_l + p_h) & 0xfffff000);
    u = t * LG2_H;
    v = (p_l - (t - p_h)) * LG2 + t * LG2_L;
    z = u + v;
    w = v - (z - u);
    t = z * z;
    t1 = z - t * (P1 + t * (P2 + t * (P3 + t * (P4 + t * P5))));
    r = (z * t1) / (t1 - 2.0f) - (w + z * w);
    z = 1.0f - (r - z);
    j = FloatBits(z);
    j += n << 23;
    if ((j >> 23) <= 0) {
        z = scalbnf(z, n); /* subnormal output */
    } else {
        z = FloatFromBits(j);
    }
    return s * z;
}
