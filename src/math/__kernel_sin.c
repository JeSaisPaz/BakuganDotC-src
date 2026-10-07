// bdc 0x08a0b858 __kernel_sin
#include "bdc.h"

/* fdlibm `__kernel_sin`: sine on `[-pi/4, pi/4]` of `x + y`; `iy == 0` means `y` is zero. Tiny
   `|x| < 2^-27` returns `x`; otherwise the `S1..S6` odd polynomial. Called by `cos` (for
   quadrants 1 and 3). The double arithmetic is libgcc soft-float in the binary. */

#define HALF 5.00000000000000000000e-01
#define S1 -1.66666666666666324348e-01 /* 0xBFC55555 55555549 */
#define S2 8.33333333332248946124e-03  /* 0x3F811111 1110F8A6 */
#define S3 -1.98412698298579493134e-04 /* 0xBF2A01A0 19C161D5 */
#define S4 2.75573137070700676789e-06  /* 0x3EC71DE3 57B1FE7D */
#define S5 -2.50507602534068634195e-08 /* 0xBE5AE5E6 8A2B9CEB */
#define S6 1.58969099521155010221e-10  /* 0x3DE5D93A 5ACFD57C */

double __kernel_sin(double x, double y, int iy)
{
    union {
        double d;
        u32 words[2]; /* little-endian: words[1] is the high word */
    } v;
    double z;
    double xz;
    double r;

    v.d = x;
    if ((v.words[1] & 0x7fffffff) < 0x3e400000) { /* |x| < 2^-27 */
        if ((int)x == 0) {
            return x; /* generate inexact */
        }
    }
    z = x * x;
    xz = x * z;
    r = S2 + z * (S3 + z * (S4 + z * (S5 + z * S6)));
    if (iy == 0) {
        return x + xz * (S1 + z * r);
    }
    return x - ((z * (HALF * y - xz * r) - y) - xz * S1);
}
