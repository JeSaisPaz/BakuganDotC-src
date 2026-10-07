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

    r.x = m->x.x * x + m->y.x * y + m->z.x * z + m->w.x * w;
    r.y = m->x.y * x + m->y.y * y + m->z.y * z + m->w.y * w;
    r.z = m->x.z * x + m->y.z * y + m->z.z * z + m->w.z * w;
    r.w = m->x.w * x + m->y.w * y + m->z.w * z + m->w.w * w;
    *out = r;
}
