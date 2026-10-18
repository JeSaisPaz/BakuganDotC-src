// bdc 0x08a2982c MathVec4Lerp
#include "bdc.h"

/* Linear interpolation of two vec4s: `out = a + (b - a) * t` (VFPU `vsub.q`/`vscl.q`/`vadd.q`). */
void MathVec4Lerp(const ScePspFVector4 *a, ScePspFVector4 *out, const ScePspFVector4 *b, float t)
{
    /* All of `a` and `b` are read before `out` is written, so `out` may alias either. */
    ScePspFVector4 va = *a;
    ScePspFVector4 vb = *b;

    out->x = VfAdd(va.x, VfMul(VfSub(vb.x, va.x), t));
    out->y = VfAdd(va.y, VfMul(VfSub(vb.y, va.y), t));
    out->z = VfAdd(va.z, VfMul(VfSub(vb.z, va.z), t));
    out->w = VfAdd(va.w, VfMul(VfSub(vb.w, va.w), t));
}
