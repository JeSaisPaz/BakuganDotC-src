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

    r.x.x = m->x.x * su;
    r.x.y = m->x.y * su;
    r.x.z = m->x.z * su;
    r.x.w = m->x.w * su;
    r.y.x = m->y.x * sv;
    r.y.y = m->y.y * sv;
    r.y.z = m->y.z * sv;
    r.y.w = m->y.w * sv;
    r.z = m->z;
    r.w.x = m->x.x * u + m->y.x * v + m->z.x * 0.0f + m->w.x * 1.0f;
    r.w.y = m->x.y * u + m->y.y * v + m->z.y * 0.0f + m->w.y * 1.0f;
    r.w.z = m->x.z * u + m->y.z * v + m->z.z * 0.0f + m->w.z * 1.0f;
    r.w.w = m->x.w * u + m->y.w * v + m->z.w * 0.0f + m->w.w * 1.0f;
    *dst = r;
    return dst;
}
