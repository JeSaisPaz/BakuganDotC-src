// bdc 0x089dd820 GmoInterpSlerpF32
#include "bdc.h"

/* Spherical linear interpolation between two float quaternion keys at `t` (mode 4 of
   `GmoMotionEvalKeyF32`). With `d` the dot product clamped to [-1, 1]: `!(d < 0.998046875f)`
   (also NaN) returns the linear blend `a + (b - a)*t`; `d < -0.998046875f` returns
   `a - (b + a)*t` (linear towards `-b`); otherwise the shorter arc (key1 negated when `d < 0`) at
   angle `theta = acos|d|` in quarter turns (`1 - |asin d|` when `|d| < sqrt(1/2)`, else
   `asin(sqrt(1 - d*d))`), weights `sin((1-t)theta)`, `sin(t*theta)` over `sin(theta)`. Returns the
   quaternion (left in VFPU C000 on the PSP). */

ScePspFVector4 GmoInterpSlerpF32(float t, const GmoMotionKeyQuatF *key0, const GmoMotionKeyQuatF *key1)
{
    float a[4];
    float b[4];
    float diff[4];
    float sum[4];
    float r[4];
    float d;
    float s;
    float theta;
    float w0;
    float w1;
    float inv;
    ScePspFVector4 out;
    s32 i;

    for (i = 0; i < 4; i++) {
        a[i] = key0->q[i];
        b[i] = key1->q[i];
    }
    d = VfSat1(VfDot4(a[0], a[1], a[2], a[3], b[0], b[1], b[2], b[3]));
    for (i = 0; i < 4; i++) {
        diff[i] = VfMul(VfSub(b[i], a[i]), t);
        sum[i] = VfMul(VfAdd(b[i], a[i]), t);
    }
    s = VfSub(1.0f, VfMul(d, d));
    if (VfDaz(d) < 0.0f) {
        for (i = 0; i < 4; i++) {
            b[i] = -b[i];
        }
    }
    if (!(VfDaz(d) < 0.998046875f)) {
        for (i = 0; i < 4; i++) {
            r[i] = VfAdd(a[i], diff[i]);
        }
    } else if (VfDaz(d) < -0.998046875f) {
        for (i = 0; i < 4; i++) {
            r[i] = VfSub(a[i], sum[i]);
        }
    } else {
        if (__builtin_fabsf(VfDaz(d)) < 0.707106769f) {
            theta = VfSub(1.0f, __builtin_fabsf(VfAsinQuarter(d)));
        } else {
            theta = VfAsinQuarter(VfSqrt(s));
        }
        w0 = VfSinQuarter(VfMul(VfSub(1.0f, t), theta));
        w1 = VfSinQuarter(VfMul(t, theta));
        inv = VfRcp(VfSinQuarter(theta));
        for (i = 0; i < 4; i++) {
            r[i] = VfMul(VfDot2(a[i], b[i], w0, w1), inv);
        }
    }
    out.x = r[0];
    out.y = r[1];
    out.z = r[2];
    out.w = r[3];
    return out;
}
