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

    r.x.x = m->x.x * s;
    r.x.y = m->x.y * s;
    r.x.z = m->x.z * s;
    r.x.w = m->x.w * s;
    r.y.x = m->y.x * s;
    r.y.y = m->y.y * s;
    r.y.z = m->y.z * s;
    r.y.w = m->y.w * s;
    r.z.x = m->z.x * s;
    r.z.y = m->z.y * s;
    r.z.z = m->z.z * s;
    r.z.w = m->z.w * s;
    r.w.x = m->x.x * v->x + m->y.x * v->y + m->z.x * v->z + m->w.x;
    r.w.y = m->x.y * v->x + m->y.y * v->y + m->z.y * v->z + m->w.y;
    r.w.z = m->x.z * v->x + m->y.z * v->y + m->z.z * v->z + m->w.z;
    r.w.w = m->x.w * v->x + m->y.w * v->y + m->z.w * v->z + m->w.w;
    *dst = r;
    return dst;
}
