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
    d = VfSat1(VfDot4(p.x, p.y, p.z, p.w, q.x, q.y, q.z, q.w));
    diff.x = VfMul(VfSub(q.x, p.x), t);
    diff.y = VfMul(VfSub(q.y, p.y), t);
    diff.z = VfMul(VfSub(q.z, p.z), t);
    diff.w = VfMul(VfSub(q.w, p.w), t);
    sum.x = VfMul(VfAdd(q.x, p.x), t);
    sum.y = VfMul(VfAdd(q.y, p.y), t);
    sum.z = VfMul(VfAdd(q.z, p.z), t);
    sum.w = VfMul(VfAdd(q.w, p.w), t);
    if (VfDaz(d) < 0.0f) {
        q.x = -q.x;
        q.y = -q.y;
        q.z = -q.z;
        q.w = -q.w;
    }
    if (!(VfDaz(d) < 0.998046875f)) {
        r.x = VfAdd(p.x, diff.x);
        r.y = VfAdd(p.y, diff.y);
        r.z = VfAdd(p.z, diff.z);
        r.w = VfAdd(p.w, diff.w);
        return r;
    }
    if (VfDaz(d) < -0.998046875f) {
        r.x = VfSub(p.x, sum.x);
        r.y = VfSub(p.y, sum.y);
        r.z = VfSub(p.z, sum.z);
        r.w = VfSub(p.w, sum.w);
        return r;
    }
    sq = VfSqrt(VfSub(1.0f, VfMul(d, d)));
    th = VfSub(1.0f, __builtin_fabsf(VfAsinQuarter(d)));
    if (!(__builtin_fabsf(VfDaz(d)) < 0.707106769f)) {
        th = VfAsinQuarter(sq);
    }
    wp = VfSinQuarter(VfMul(VfSub(1.0f, t), th));
    wq = VfSinQuarter(VfMul(t, th));
    inv = VfRcp(VfSinQuarter(th));
    r.x = VfMul(VfDot2(p.x, q.x, wp, wq), inv);
    r.y = VfMul(VfDot2(p.y, q.y, wp, wq), inv);
    r.z = VfMul(VfDot2(p.z, q.z, wp, wq), inv);
    r.w = VfMul(VfDot2(p.w, q.w, wp, wq), inv);
    return r;
}
