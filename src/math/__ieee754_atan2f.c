// bdc 0x08a092c8 __ieee754_atan2f
#include "bdc.h"

/* fdlibm/newlib `__ieee754_atan2f`: single-precision `atan2(y, x)`, worker behind `atan2f`. NaN
   inputs return `x+y`, `x == 1.0` short-cuts to `atanf``(y)`, then the usual quadrant table `m =
   (sign y) | 2*(sign x)` with pi/pi_lo corrections; uses `fabsf`. This build returns +0 (not -0)
   for `x == +inf`, finite `y < 0`. */
float __ieee754_atan2f(float y, float x)
{
    static const float pi = 3.1415925026e+00f;     /* 0x40490fda */
    static const float piO2 = 1.5707963705e+00f;   /* 0x3fc90fdb */
    static const float piO4 = 7.8539818525e-01f;   /* 0x3f490fdb */
    static const float pi3O4 = 2.3561944962e+00f;  /* 0x4016cbe4 */
    static const float piLo = 1.5099578832e-07f;   /* 0x34222168 */
    union { float f; s32 i; } bits;
    s32 hx, ix, hy, iy, k;
    int m;
    float z;

    bits.f = x;
    hx = bits.i;
    ix = hx & 0x7fffffff;
    bits.f = y;
    hy = bits.i;
    iy = hy & 0x7fffffff;
    if (ix > 0x7f800000 || iy > 0x7f800000) {
        return x + y; /* x or y is NaN */
    }
    if (hx == 0x3f800000) {
        return atanf(y); /* x == 1.0 */
    }
    m = ((hy >> 31) & 1) | ((hx >> 30) & 2); /* 2*sign(x) + sign(y) */

    if (iy == 0) {
        switch (m) {
        case 0:
        case 1:
            return y; /* atan(+-0, +anything) = +-0 */
        case 2:
            return pi; /* atan(+0, -anything) = pi */
        case 3:
            return -pi; /* atan(-0, -anything) = -pi */
        }
    }
    if (ix == 0) {
        return (hy < 0) ? -piO2 : piO2;
    }
    if (ix == 0x7f800000) {
        if (iy == 0x7f800000) {
            switch (m) {
            case 0:
                return piO4;
            case 1:
                return -piO4;
            case 2:
                return pi3O4;
            case 3:
                return -pi3O4;
            }
        } else {
            switch (m) {
            case 0:
            case 1:
                return 0.0f;
            case 2:
                return pi;
            case 3:
                return -pi;
            }
        }
    }
    if (iy == 0x7f800000) {
        return (hy < 0) ? -piO2 : piO2;
    }

    k = (iy - ix) >> 23;
    if (k > 60) {
        z = 1.5707964897e+00f; /* |y/x| > 2^60: pi/2 + 0.5 * pi_lo */
    } else if (hx < 0 && k < -60) {
        z = 0.0f; /* |y|/x < -2^60 */
    } else {
        z = atanf(fabsf(y / x));
    }
    switch (m) {
    case 0:
        return z; /* atan(+, +) */
    case 1:
        bits.f = z; /* atan(-, +): flip the sign bit */
        bits.i ^= (s32)0x80000000;
        return bits.f;
    case 2:
        return pi - (z - piLo); /* atan(+, -) */
    default:
        return (z - piLo) - pi; /* atan(-, -) */
    }
}
