// bdc 0x08a0bb60 __kernel_rem_pio2f
#include "bdc.h"

/* fdlibm/newlib `__kernel_rem_pio2f` (`k_rem_pio2f.c`): single-precision version of
   `__kernel_rem_pio2` for huge arguments of `__ieee754_rem_pio2f`. `x[0..nx-1]` are 8-bit
   chunks of the input scaled by 2^-e0, `ipio2` the 8-bit chunks of 2/pi (`g_twoOverPiF`);
   it multiplies them, keeps the fraction with enough terms (`jk` from
   `g_kernelRemPio2fInitJk`, recomputing with more terms when the leading bits cancel),
   multiplies it back by pi/2 (`g_kernelRemPio2fPIo2`) and writes 1 (`prec` 0), 2 (1, 2) or
   3 (3) floats to `y`; returns the quadrant `n & 7`. As in newlib (and unlike fdlibm's
   double version), the `q0 == 0` case takes `ih = iq[jz-1] >> 8`. Uses `scalbnf` and
   `floorf`. */

#define TWO8 256.0f        /* 0x43800000 */
#define TWON8 0.00390625f  /* 0x3b800000, 2^-8 */

int __kernel_rem_pio2f(float *x, float *y, int e0, int nx, int prec, const int *ipio2)
{
    s32 jz, jx, jv, jp, jk, carry, n, iq[20], i, j, k, m, q0, ih;
    float z, fw, f[20], fq[20], q[20];

    /* initialize jk */
    jk = g_kernelRemPio2fInitJk[prec];
    jp = jk;

    /* determine jx, jv, q0; note that 3 > q0 */
    jx = nx - 1;
    jv = (e0 - 3) / 8;
    if (jv < 0) {
        jv = 0;
    }
    q0 = e0 - 8 * (jv + 1);

    /* set up f[0] to f[jx+jk] where f[jx+jk] = ipio2[jv+jk] */
    j = jv - jx;
    m = jx + jk;
    for (i = 0; i <= m; i++, j++) {
        f[i] = (j < 0) ? 0.0f : (float)ipio2[j];
    }

    /* compute q[0], q[1], ... q[jk] */
    for (i = 0; i <= jk; i++) {
        for (j = 0, fw = 0.0f; j <= jx; j++) {
            fw += x[j] * f[jx + i - j];
        }
        q[i] = fw;
    }

    jz = jk;
    for (;;) {
        /* distill q[] into iq[] reversingly */
        for (i = 0, j = jz, z = q[jz]; j > 0; i++, j--) {
            fw = (float)(s32)(z * TWON8);
            iq[i] = (s32)(z - fw * TWO8);
            z = q[j - 1] + fw;
        }

        /* compute n */
        z = scalbnf(z, q0);                   /* actual value of z */
        z -= floorf(z * 0.125f) * 8.0f;       /* trim off integer >= 8 */
        n = (s32)z;
        z -= (float)n;
        ih = 0;
        if (q0 > 0) { /* need iq[jz-1] to determine n */
            i = iq[jz - 1] >> (8 - q0);
            n += i;
            iq[jz - 1] -= i << (8 - q0);
            ih = iq[jz - 1] >> (7 - q0);
        } else if (q0 == 0) {
            ih = iq[jz - 1] >> 8;
        } else if (!(z < 0.5f)) {
            ih = 2;
        }

        if (ih > 0) { /* q > 0.5 */
            n += 1;
            carry = 0;
            for (i = 0; i < jz; i++) { /* compute 1 - q */
                j = iq[i];
                if (carry == 0) {
                    if (j != 0) {
                        carry = 1;
                        iq[i] = 0x100 - j;
                    }
                } else {
                    iq[i] = 0xff - j;
                }
            }
            if (q0 > 0) { /* rare case: chance is 1 in 12 */
                switch (q0) {
                case 1:
                    iq[jz - 1] &= 0x7f;
                    break;
                case 2:
                    iq[jz - 1] &= 0x3f;
                    break;
                }
            }
            if (ih == 2) {
                z = 1.0f - z;
                if (carry != 0) {
                    z -= scalbnf(1.0f, q0);
                }
            }
        }

        /* check if recomputation is needed */
        if (!(z == 0.0f)) {
            break;
        }
        j = 0;
        for (i = jz - 1; i >= jk; i--) {
            j |= iq[i];
        }
        if (j != 0) {
            break;
        }
        /* k = number of additional terms needed */
        for (k = 1; iq[jk - k] == 0; k++) {
        }
        for (i = jz + 1; i <= jz + k; i++) { /* add q[jz+1] to q[jz+k] */
            f[jx + i] = (float)ipio2[jv + i];
            for (j = 0, fw = 0.0f; j <= jx; j++) {
                fw += x[j] * f[jx + i - j];
            }
            q[i] = fw;
        }
        jz += k;
    }

    /* chop off zero terms */
    if (z == 0.0f) {
        jz -= 1;
        q0 -= 8;
        while (iq[jz] == 0) {
            jz--;
            q0 -= 8;
        }
    } else { /* break z into 8-bit chunks if necessary */
        z = scalbnf(z, -q0);
        if (!(z < TWO8)) {
            fw = (float)(s32)(z * TWON8);
            iq[jz] = (s32)(z - fw * TWO8);
            jz += 1;
            q0 += 8;
            iq[jz] = (s32)fw;
        } else {
            iq[jz] = (s32)z;
        }
    }

    /* convert the integer "bit" chunks to floating-point values */
    fw = scalbnf(1.0f, q0);
    for (i = jz; i >= 0; i--) {
        q[i] = fw * (float)iq[i];
        fw *= TWON8;
    }

    /* compute PIo2[0..jp] * q[jz..0] */
    for (i = jz; i >= 0; i--) {
        for (fw = 0.0f, k = 0; k <= jp && k <= jz - i; k++) {
            fw += g_kernelRemPio2fPIo2[k] * q[i + k];
        }
        fq[jz - i] = fw;
    }

    /* compress fq[] into y[] */
    switch (prec) {
    case 0:
        fw = 0.0f;
        for (i = jz; i >= 0; i--) {
            fw += fq[i];
        }
        y[0] = (ih == 0) ? fw : -fw;
        break;
    case 1:
    case 2:
        fw = 0.0f;
        for (i = jz; i >= 0; i--) {
            fw += fq[i];
        }
        y[0] = (ih == 0) ? fw : -fw;
        fw = fq[0] - fw;
        for (i = 1; i <= jz; i++) {
            fw += fq[i];
        }
        y[1] = (ih == 0) ? fw : -fw;
        break;
    case 3: /* painful */
        for (i = jz; i > 0; i--) {
            fw = fq[i - 1] + fq[i];
            fq[i] += fq[i - 1] - fw;
            fq[i - 1] = fw;
        }
        for (i = jz; i > 1; i--) {
            fw = fq[i - 1] + fq[i];
            fq[i] += fq[i - 1] - fw;
            fq[i - 1] = fw;
        }
        for (fw = 0.0f, i = jz; i >= 2; i--) {
            fw += fq[i];
        }
        if (ih == 0) {
            y[0] = fq[0];
            y[1] = fq[1];
            y[2] = fw;
        } else {
            y[0] = -fq[0];
            y[1] = -fq[1];
            y[2] = -fw;
        }
        break;
    }
    return n & 7;
}
