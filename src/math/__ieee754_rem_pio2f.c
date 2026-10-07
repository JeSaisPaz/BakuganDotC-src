// bdc 0x08a0a220 __ieee754_rem_pio2f
#include "bdc.h"

/* fdlibm/newlib `__ieee754_rem_pio2f`: single-precision argument reduction modulo pi/2 for
   `tanf`: writes `y[0]`+`y[1]` and returns the quadrant. Small arguments use the `pio2_1f`..
   constants and the `g_npio2HwF` table; large ones call `__kernel_rem_pio2f` with
   `g_twoOverPiF`. Unlike newlib, infinities and NaN give `y[0] = y[1] = 0` (not `x - x`). */

#define INVPIO2 0x1.45f308p-1f  /* 6.3661980629e-01, 0x3f22f984 */
#define PIO2_1 0x1.921fp+0f     /* 1.5707855225e+00, 0x3fc90f80 first 17 bits of pi/2 */
#define PIO2_1T 0x1.6a8886p-17f /* 1.0804334124e-05, 0x37354443 pi/2 - PIO2_1 */
#define PIO2_2 0x1.6a88p-17f    /* 1.0804273188e-05, 0x37354400 second 17 bits */
#define PIO2_2T 0x1.0b461p-34f  /* 6.0770999344e-11, 0x2e85a308 pi/2 - (PIO2_1 + PIO2_2) */
#define PIO2_3 0x1.0b46p-34f    /* 6.0770943833e-11, 0x2e85a300 third 17 bits */
#define PIO2_3T 0x1.1a6264p-54f /* 6.1232342629e-17, 0x248d3132 */
#define TWO8 256.0f

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

int __ieee754_rem_pio2f(float x, float *y)
{
    s32 hx = FloatBits(x);
    s32 ix = hx & 0x7fffffff;

    if (ix <= 0x3f490fd8) { /* |x| ~<= pi/4, no reduction needed */
        y[0] = x;
        y[1] = 0.0f;
        return 0;
    }
    if (ix < 0x4016cbe4) { /* |x| < 3pi/4, special case with n = +-1 */
        float z;

        if (hx > 0) {
            z = x - PIO2_1;
            if ((ix & 0xfffffff0) != 0x3fc90fd0) { /* 24+24 bit pi OK */
                y[0] = z - PIO2_1T;
                y[1] = (z - y[0]) - PIO2_1T;
            } else { /* near pi/2, use 24+24+24 bit pi */
                z -= PIO2_2;
                y[0] = z - PIO2_2T;
                y[1] = (z - y[0]) - PIO2_2T;
            }
            return 1;
        }
        z = x + PIO2_1;
        if ((ix & 0xfffffff0) != 0x3fc90fd0) {
            y[0] = z + PIO2_1T;
            y[1] = (z - y[0]) + PIO2_1T;
        } else {
            z += PIO2_2;
            y[0] = z + PIO2_2T;
            y[1] = (z - y[0]) + PIO2_2T;
        }
        return -1;
    }
    if (ix <= 0x43490f80) { /* |x| ~<= 2^7 * (pi/2), medium size */
        float t = fabsf(x);
        int n = (int)(t * INVPIO2 + 0.5f);
        float fn = (float)n;
        float r = t - fn * PIO2_1;
        float w = fn * PIO2_1T; /* 1st round good to 40 bits */

        if (n < 32 && (u32)(ix & 0xffffff00) != g_npio2HwF[n - 1]) {
            y[0] = r - w; /* quick check: no cancellation */
        } else {
            int j = ix >> 23;

            y[0] = r - w;
            if (j - ((FloatBits(y[0]) >> 23) & 0xff) > 8) { /* 2nd iteration, good to 57 bits */
                t = r;
                w = fn * PIO2_2;
                r = t - w;
                w = fn * PIO2_2T - ((t - r) - w);
                y[0] = r - w;
                if (j - ((FloatBits(y[0]) >> 23) & 0xff) > 25) { /* 3rd iteration, 74 bits */
                    t = r;
                    w = fn * PIO2_3;
                    r = t - w;
                    w = fn * PIO2_3T - ((t - r) - w);
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
    if (ix >= 0x7f800000) { /* inf or NaN */
        y[1] = 0.0f;
        y[0] = 0.0f;
        return 0;
    }
    {
        /* Large: split |x| scaled to [2^7, 2^8) into three 8-bit chunks. */
        float tx[3];
        int e0 = (ix >> 23) - 134; /* e0 = ilogb(z) - 7 */
        float z = FloatFromBits(ix - (e0 << 23));
        int nx;
        int i;
        int n;

        for (i = 0; i < 2; i++) {
            tx[i] = (float)(int)z;
            z = (z - tx[i]) * TWO8;
        }
        tx[2] = z;
        nx = 3;
        while (tx[nx - 1] == 0.0f) { /* skip zero terms */
            nx--;
        }
        n = __kernel_rem_pio2f(tx, y, e0, nx, 2, g_twoOverPiF);
        if (hx < 0) {
            y[0] = -y[0];
            y[1] = -y[1];
            return -n;
        }
        return n;
    }
}
