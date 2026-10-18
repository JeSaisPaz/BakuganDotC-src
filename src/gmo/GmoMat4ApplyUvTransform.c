// bdc 0x089dd60c GmoMat4ApplyUvTransform
#include "bdc.h"

/* Applies a UV transform record `{uOffset, vOffset, uScale, vScale}` to a texture matrix: scales
   column `x` by `uv[2]` and column `y` by `uv[3]`, keeps column `z`, and sets the translation
   column to `m * (uOffset, vOffset, 0, 1)` (`vtfm4.q` on E100; the masked `vidt.q` supplies the
   0 and 1, and the `z` term is kept with its 0 factor as the hardware multiplies it). Everything
   is read from `m` before `dst` is written, so `dst` may alias `m`. Returns `dst`. */
ScePspFMatrix4 *GmoMat4ApplyUvTransform(ScePspFMatrix4 *dst, const ScePspFMatrix4 *m, const float *uv)
{
    ScePspFMatrix4 r;
    float u = uv[0];
    float v = uv[1];
    float su = uv[2];
    float sv = uv[3];

    r.x.x = VfMul(m->x.x, su);
    r.x.y = VfMul(m->x.y, su);
    r.x.z = VfMul(m->x.z, su);
    r.x.w = VfMul(m->x.w, su);
    r.y.x = VfMul(m->y.x, sv);
    r.y.y = VfMul(m->y.y, sv);
    r.y.z = VfMul(m->y.z, sv);
    r.y.w = VfMul(m->y.w, sv);
    r.z = m->z;
    r.w.x = VfDot4(m->x.x, m->y.x, m->z.x, m->w.x, u, v, 0.0f, 1.0f);
    r.w.y = VfDot4(m->x.y, m->y.y, m->z.y, m->w.y, u, v, 0.0f, 1.0f);
    r.w.z = VfDot4(m->x.z, m->y.z, m->z.z, m->w.z, u, v, 0.0f, 1.0f);
    r.w.w = VfDot4(m->x.w, m->y.w, m->z.w, m->w.w, u, v, 0.0f, 1.0f);
    *dst = r;
    return dst;
}
