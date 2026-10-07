// bdc 0x08a2982c MathVec4Lerp
#include "bdc.h"

/* Linear interpolation of two vec4s: `out = a + (b - a) * t` (VFPU `vsub.q`/`vscl.q`/`vadd.q`). */
void MathVec4Lerp(const ScePspFVector4 *a, ScePspFVector4 *out, const ScePspFVector4 *b, float t)
{
    /* All of `a` and `b` are read before `out` is written, so `out` may alias either. */
    ScePspFVector4 va = *a;
    ScePspFVector4 vb = *b;

    out->x = va.x + (vb.x - va.x) * t;
    out->y = va.y + (vb.y - va.y) * t;
    out->z = va.z + (vb.z - va.z) * t;
    out->w = va.w + (vb.w - va.w) * t;
}
