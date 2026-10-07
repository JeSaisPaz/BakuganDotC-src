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
    d = VfSat1(a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3]);
    for (i = 0; i < 4; i++) {
        diff[i] = (b[i] - a[i]) * t;
        sum[i] = (b[i] + a[i]) * t;
    }
    s = 1.0f - d * d;
    if (d < 0.0f) {
        for (i = 0; i < 4; i++) {
            b[i] = -b[i];
        }
    }
    if (!(d < 0.998046875f)) {
        for (i = 0; i < 4; i++) {
            r[i] = a[i] + diff[i];
        }
    } else if (d < -0.998046875f) {
        for (i = 0; i < 4; i++) {
            r[i] = a[i] - sum[i];
        }
    } else {
        if (__builtin_fabsf(d) < 0.707106769f) {
            theta = 1.0f - __builtin_fabsf(VfAsinQuarter(d));
        } else {
            theta = VfAsinQuarter(__builtin_sqrtf(s));
        }
        w0 = VfSinQuarter((1.0f - t) * theta);
        w1 = VfSinQuarter(t * theta);
        inv = VfRcp(VfSinQuarter(theta));
        for (i = 0; i < 4; i++) {
            r[i] = (a[i] * w0 + b[i] * w1) * inv;
        }
    }
    out.x = r[0];
    out.y = r[1];
    out.z = r[2];
    out.w = r[3];
    return out;
}
