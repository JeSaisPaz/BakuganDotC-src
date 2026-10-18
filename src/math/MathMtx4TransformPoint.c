// bdc 0x08a29804 MathMtx4TransformPoint
#include "bdc.h"

/* Transforms the point `p` (x, y, z; w forced to 1.0) by the 4x4 matrix `m` (`vtfm4.q` with `E100`:
   out = p.x * m->x + p.y * m->y + p.z * m->z + 1.0 * m->w) and stores the full vec4 result in `out`.
   `p` is read before `out` is written, so `out` may alias `p`. */
void MathMtx4TransformPoint(const ScePspFMatrix4 *m, ScePspFVector4 *out, const ScePspFVector4 *p)
{
    float x = p->x;
    float y = p->y;
    float z = p->z;
    float w = 1.0f;
    ScePspFVector4 r;

    r.x = VfDot4(m->x.x, m->y.x, m->z.x, m->w.x, x, y, z, w);
    r.y = VfDot4(m->x.y, m->y.y, m->z.y, m->w.y, x, y, z, w);
    r.z = VfDot4(m->x.z, m->y.z, m->z.z, m->w.z, x, y, z, w);
    r.w = VfDot4(m->x.w, m->y.w, m->z.w, m->w.w, x, y, z, w);
    *out = r;
}
