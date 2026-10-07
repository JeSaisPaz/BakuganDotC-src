// bdc 0x08a0823c __ieee754_rem_pio2
#include "bdc.h"

/* fdlibm/newlib `__ieee754_rem_pio2` (`e_rem_pio2.c`): reduces `x` modulo pi/2, writes the
   remainder as the double pair `y[0] + y[1]` and returns the quadrant number `n`. Called by `cos`.
   `|x| <= pi/4` is returned as is; `|x| < 3pi/4` subtracts one pi/2 (`pio2_1`/`pio2_1t`, with
   `pio2_2` when `x` is close to pi/2); `|x| <= 2^19 pi/2` uses `n = rint(|x| 2/pi)` with up to three
   pi/2 pieces (`g_npio2Hw` spots cancellation); larger finite arguments are split into 24-bit
   chunks for `__kernel_rem_pio2` with `g_twoOverPi`. Unlike fdlibm, Inf/NaN return
   `y[0] = y[1] = 0` (this build loads the constant zero instead of computing `x - x`). Constants
   from `g_remPio2Consts`. */
int __ieee754_rem_pio2(double x, double *y)
{
    const RemPio2Consts *k = &g_remPio2Consts;
    union {
        double d;
        u32 words[2]; /* little-endian: words[0] low, words[1] high */
    } value;
    double z;
    double w;
    double t;
    double r;
    double fn;
    double tx[3];
    s32 e0;
    s32 i;
    s32 j;
    s32 nx;
    s32 n;
    s32 ix;
    s32 hx;

    value.d = x;
    hx = (s32)value.words[1];
    ix = hx & 0x7fffffff;
    if (ix <= 0x3fe921fb) { /* |x| ~<= pi/4, no reduction needed */
        y[0] = x;
        y[1] = k->zero;
        return 0;
    }
    if (ix < 0x4002d97c) { /* |x| < 3pi/4, special case with n = +-1 */
        if (hx > 0) {
            z = x - k->pio2_1;
            if (ix != 0x3ff921fb) { /* 33+53 bit pi is good enough */
                y[0] = z - k->pio2_1t;
                y[1] = (z - y[0]) - k->pio2_1t;
            } else { /* near pi/2, use 33+33+53 bit pi */
                z -= k->pio2_2;
                y[0] = z - k->pio2_2t;
                y[1] = (z - y[0]) - k->pio2_2t;
            }
            return 1;
        }
        z = x + k->pio2_1;
        if (ix != 0x3ff921fb) {
            y[0] = z + k->pio2_1t;
            y[1] = (z - y[0]) + k->pio2_1t;
        } else {
            z += k->pio2_2;
            y[0] = z + k->pio2_2t;
            y[1] = (z - y[0]) + k->pio2_2t;
        }
        return -1;
    }
    if (ix <= 0x413921fb) { /* |x| ~<= 2^19 (pi/2), medium size */
        t = fabs(x);
        n = (s32)(t * k->invpio2 + k->half);
        fn = (double)n;
        r = t - fn * k->pio2_1;
        w = fn * k->pio2_1t; /* 1st round, good to 85 bits */
        if (n < 32 && ix != g_npio2Hw[n - 1]) {
            y[0] = r - w; /* quick check: no cancellation */
        } else {
            j = ix >> 20;
            y[0] = r - w;
            value.d = y[0];
            i = j - (s32)((value.words[1] >> 20) & 0x7ff);
            if (i > 16) { /* 2nd iteration needed, good to 118 bits */
                t = r;
                w = fn * k->pio2_2;
                r = t - w;
                w = fn * k->pio2_2t - ((t - r) - w);
                y[0] = r - w;
                value.d = y[0];
                i = j - (s32)((value.words[1] >> 20) & 0x7ff);
                if (i > 49) { /* 3rd iteration, 151 bits */
                    t = r;
                    w = fn * k->pio2_3;
                    r = t - w;
                    w = fn * k->pio2_3t - ((t - r) - w);
                    y[0] = r - w;
                }
            }
        }
        y[1] = (r - y[0]) - w;
        if (hx < 0) {
            y[0] = -y[0];
            y[1] = -y[1];
            return -n;
        }
        return n;
    }
    if (ix >= 0x7ff00000) { /* Inf or NaN */
        y[0] = k->zero;
        y[1] = k->zero;
        return 0;
    }
    /* all other (large) arguments: z = scalbn(|x|, -ilogb(x) + 23) */
    e0 = (ix >> 20) - 1046;
    value.d = x;
    value.words[1] = (u32)(ix - (e0 << 20));
    z = value.d;
    for (i = 0; i < 2; i++) {
        tx[i] = (double)(s32)z;
        z = (z - tx[i]) * k->two24;
    }
    tx[2] = z;
    nx = 3;
    while (tx[nx - 1] == k->zero) { /* skip zero terms */
        nx--;
    }
    n = __kernel_rem_pio2(tx, y, e0, nx, 2, g_twoOverPi);
    if (hx < 0) {
        y[0] = -y[0];
        y[1] = -y[1];
        return -n;
    }
    return n;
}
