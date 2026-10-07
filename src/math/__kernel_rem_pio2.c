// bdc 0x08a0ab88 __kernel_rem_pio2
#include "bdc.h"

/* fdlibm `__kernel_rem_pio2` (`k_rem_pio2.c`): multi-precision reduction of huge arguments modulo
   pi/2. `x[0..nx-1]` are 24-bit chunks of the input scaled by `2^-e0`, `ipio2` the 24-bit chunks of
   2/pi (`g_twoOverPi`); writes the remainder to `y` as 1 (`prec` 0), 2 (`prec` 1, 2) or 3 (`prec`
   3) doubles and returns the quadrant `n & 7`. Used by `__ieee754_rem_pio2` (its only caller).
   Calls `scalbn` and the second copy of floor (`floor_libm`); constants from
   `g_kernelRemPio2InitJk`, `g_kernelRemPio2PIo2` and `g_kernelRemPio2Consts`. */
int __kernel_rem_pio2(double *x, double *y, int e0, int nx, int prec, const int *ipio2)
{
    const KernelRemPio2Consts *k = &g_kernelRemPio2Consts;
    s32 iq[20];
    double f[20];
    double fq[20];
    double q[20];
    double z;
    double fw;
    s32 jz;
    s32 jx;
    s32 jv;
    s32 jp;
    s32 jk;
    s32 carry;
    s32 n;
    s32 i;
    s32 j;
    s32 m;
    s32 q0;
    s32 ih;
    s32 more;

    /* initialize jk */
    jk = g_kernelRemPio2InitJk[prec];
    jp = jk;

    /* determine jx, jv, q0; note that 3 > q0 */
    jx = nx - 1;
    jv = (e0 - 3) / 24;
    if (jv < 0) {
        jv = 0;
    }
    q0 = e0 - 24 * (jv + 1);

    /* set up f[0] to f[jx + jk] where f[jx + jk] = ipio2[jv + jk] */
    j = jv - jx;
    m = jx + jk;
    for (i = 0; i <= m; i++, j++) {
        f[i] = (j < 0) ? k->zero : (double)ipio2[j];
    }

    /* compute q[0], q[1], ... q[jk] */
    for (i = 0; i <= jk; i++) {
        for (j = 0, fw = k->zero; j <= jx; j++) {
            fw += x[j] * f[jx + i - j];
        }
        q[i] = fw;
    }

    jz = jk;
    do {
        more = 0;
        /* distill q[] into iq[] reversingly */
        z = q[jz];
        for (i = 0, j = jz; j > 0; i++, j--) {
            fw = (double)(s32)(k->twon24 * z);
            iq[i] = (s32)(z - k->two24 * fw);
            z = q[j - 1] + fw;
        }

        /* compute n */
        z = scalbn(z, q0);                         /* actual value of z */
        z -= k->eight * floor_libm(z * k->eighth); /* trim off integer >= 8 */
        n = (s32)z;
        z -= (double)n;
        ih = 0;
        if (q0 > 0) { /* need iq[jz-1] to determine n */
            i = iq[jz - 1] >> (24 - q0);
            n += i;
            iq[jz - 1] -= i << (24 - q0);
            ih = iq[jz - 1] >> (23 - q0);
        } else if (q0 == 0) {
            ih = iq[jz - 1] >> 23;
        } else if (z >= k->half) {
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
                        iq[i] = 0x1000000 - j;
                    }
                } else {
                    iq[i] = 0xffffff - j;
                }
            }
            if (q0 > 0) { /* rare case: chance is 1 in 12 */
                switch (q0) {
                case 1:
                    iq[jz - 1] &= 0x7fffff;
                    break;
                case 2:
                    iq[jz - 1] &= 0x3fffff;
                    break;
                }
            }
            if (ih == 2) {
                z = k->one - z;
                if (carry != 0) {
                    z -= scalbn(k->one, q0);
                }
            }
        }

        /* check if recomputation is needed */
        if (z == k->zero) {
            j = 0;
            for (i = jz - 1; i >= jk; i--) {
                j |= iq[i];
            }
            if (j == 0) { /* need recomputation */
                s32 extra;

                for (extra = 1; iq[jk - extra] == 0; extra++) {
                    /* extra = number of terms needed */
                }
                for (i = jz + 1; i <= jz + extra; i++) { /* add q[jz+1] to q[jz+extra] */
                    f[jx + i] = (double)ipio2[jv + i];
                    for (j = 0, fw = k->zero; j <= jx; j++) {
                        fw += x[j] * f[jx + i - j];
                    }
                    q[i] = fw;
                }
                jz += extra;
                more = 1;
            }
        }
    } while (more);

    /* chop off zero terms */
    if (z == k->zero) {
        jz -= 1;
        q0 -= 24;
        while (iq[jz] == 0) {
            jz--;
            q0 -= 24;
        }
    } else { /* break z into 24-bit if necessary */
        z = scalbn(z, -q0);
        if (z >= k->two24) {
            fw = (double)(s32)(k->twon24 * z);
            iq[jz] = (s32)(z - k->two24 * fw);
            jz += 1;
            q0 += 24;
            iq[jz] = (s32)fw;
        } else {
            iq[jz] = (s32)z;
        }
    }

    /* convert integer "bit" chunk to floating-point value */
    fw = scalbn(k->one, q0);
    for (i = jz; i >= 0; i--) {
        q[i] = fw * (double)iq[i];
        fw *= k->twon24;
    }

    /* compute PIo2[0, ..., jp] * q[jz, ..., 0] */
    for (i = jz; i >= 0; i--) {
        s32 t;

        for (fw = k->zero, t = 0; t <= jp && t <= jz - i; t++) {
            fw += g_kernelRemPio2PIo2[t] * q[i + t];
        }
        fq[jz - i] = fw;
    }

    /* compress fq[] into y[] */
    switch (prec) {
    case 0:
        fw = k->zero;
        for (i = jz; i >= 0; i--) {
            fw += fq[i];
        }
        y[0] = (ih == 0) ? fw : -fw;
        break;
    case 1:
    case 2:
        fw = k->zero;
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
        for (fw = k->zero, i = jz; i >= 2; i--) {
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
