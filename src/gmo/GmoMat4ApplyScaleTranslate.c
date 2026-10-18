// bdc 0x089dd5d0 GmoMat4ApplyScaleTranslate
#include "bdc.h"

/* Scales the three axis columns `x`, `y`, `z` of `m` by `v.w` and sets the translation column to
   `m * (v.x, v.y, v.z, 1)` (`vhtfm4.q` on E100: `w.i = x.i*v.x + y.i*v.y + z.i*v.z + w.i`);
   `v.w` is only the scale. Everything is read from `m` before `dst` is written, so `dst` may
   alias `m`. Returns `dst`. Used for the texture matrix. */
ScePspFMatrix4 *GmoMat4ApplyScaleTranslate(ScePspFMatrix4 *dst, const ScePspFMatrix4 *m, const ScePspFVector4 *v)
{
    ScePspFMatrix4 r;
    float s = v->w;

    r.x.x = VfMul(m->x.x, s);
    r.x.y = VfMul(m->x.y, s);
    r.x.z = VfMul(m->x.z, s);
    r.x.w = VfMul(m->x.w, s);
    r.y.x = VfMul(m->y.x, s);
    r.y.y = VfMul(m->y.y, s);
    r.y.z = VfMul(m->y.z, s);
    r.y.w = VfMul(m->y.w, s);
    r.z.x = VfMul(m->z.x, s);
    r.z.y = VfMul(m->z.y, s);
    r.z.z = VfMul(m->z.z, s);
    r.z.w = VfMul(m->z.w, s);
    r.w.x = VfDot4(m->x.x, m->y.x, m->z.x, m->w.x, v->x, v->y, v->z, 1.0f);
    r.w.y = VfDot4(m->x.y, m->y.y, m->z.y, m->w.y, v->x, v->y, v->z, 1.0f);
    r.w.z = VfDot4(m->x.z, m->y.z, m->z.z, m->w.z, v->x, v->y, v->z, 1.0f);
    r.w.w = VfDot4(m->x.w, m->y.w, m->z.w, m->w.w, v->x, v->y, v->z, 1.0f);
    *dst = r;
    return dst;
}
