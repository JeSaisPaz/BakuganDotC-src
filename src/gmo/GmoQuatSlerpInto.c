// bdc 0x089dddd8 GmoQuatSlerpInto
#include "bdc.h"

/* Slerps from the quaternion at `dst` (read only) toward `q` by `t` and returns the result: `q` is
   negated when the clamped dot product is negative (shortest arc); when the dot is >= 0.998046875
   (or NaN) the result is the lerp p + (q - p) * t, when it is < -0.998046875 the lerp p - (q + p) * t
   with the original `q`; otherwise (p*sin((1-t)th) + q*sin(t th)) / sin(th) with th = acos(|dot|),
   taken from asin(|dot|) below sqrt(1/2) and from asin(sqrt(1 - dot^2)) above. Used by
   `GmoMotionBlendValue` for rotation channels. */
ScePspFVector4 GmoQuatSlerpInto(float t, float *dst, ScePspFVector4 q)
{
    ScePspFVector4 p;
    ScePspFVector4 r;
    ScePspFVector4 diff;
    ScePspFVector4 sum;
    float d;
    float sq;
    float th;
    float wp;
    float wq;
    float inv;

    p.x = dst[0];
    p.y = dst[1];
    p.z = dst[2];
    p.w = dst[3];
    d = VfSat1(p.x * q.x + p.y * q.y + p.z * q.z + p.w * q.w);
    diff.x = (q.x - p.x) * t;
    diff.y = (q.y - p.y) * t;
    diff.z = (q.z - p.z) * t;
    diff.w = (q.w - p.w) * t;
    sum.x = (q.x + p.x) * t;
    sum.y = (q.y + p.y) * t;
    sum.z = (q.z + p.z) * t;
    sum.w = (q.w + p.w) * t;
    if (d < 0.0f) {
        q.x = -q.x;
        q.y = -q.y;
        q.z = -q.z;
        q.w = -q.w;
    }
    if (!(d < 0.998046875f)) {
        r.x = p.x + diff.x;
        r.y = p.y + diff.y;
        r.z = p.z + diff.z;
        r.w = p.w + diff.w;
        return r;
    }
    if (d < -0.998046875f) {
        r.x = p.x - sum.x;
        r.y = p.y - sum.y;
        r.z = p.z - sum.z;
        r.w = p.w - sum.w;
        return r;
    }
    sq = __builtin_sqrtf(1.0f - d * d);
    th = 1.0f - __builtin_fabsf(VfAsinQuarter(d));
    if (!(__builtin_fabsf(d) < 0.707106769f)) {
        th = VfAsinQuarter(sq);
    }
    wp = VfSinQuarter((1.0f - t) * th);
    wq = VfSinQuarter(t * th);
    inv = VfRcp(VfSinQuarter(th));
    r.x = (p.x * wp + q.x * wq) * inv;
    r.y = (p.y * wp + q.y * wq) * inv;
    r.z = (p.z * wp + q.z * wq) * inv;
    r.w = (p.w * wp + q.w * wq) * inv;
    return r;
}
