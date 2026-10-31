// bdc 0x08a13cbc GmoMat4MulOffsetScale
#include "bdc.h"

/* One field of the product: c0 * p->x + c1 * p->y + c2 * p->z + c3 * p->w, lane by lane, summed
   left to right. */
static ScePspFVector4 ComposeField(const ScePspFMatrix4 *p, float c0, float c1, float c2, float c3)
{
    ScePspFVector4 r;

    VfDot4x4(&r.x, &p->x.x, &p->y.x, &p->z.x, &p->w.x, c0, c1, c2, c3); /* the dot's s and t commute */
    return r;
}

/* Same code as `GmoMat4ComposeLocal` (a second copy in the binary). Composes a local matrix with a scale/offset vector `s` and its parent: builds `L'` whose
   x/y/z fields are those of `local` scaled by `s->w` (`vscl.q`) and whose w field is `local`
   applied to the point `s.xyz` (`vhtfm4.q`, row-vector convention), then
   `dst.<c> = Σ_k L'.<c>[k] · parent.<k>` (`vmmul.q E000, E200, E100`), i.e. row-major `L' · parent`.
   All inputs are read before `dst` is written, so `dst` may alias them.
   Returns `dst`. */
ScePspFMatrix4 *GmoMat4MulOffsetScale(ScePspFMatrix4 *dst, const ScePspFMatrix4 *parent, const ScePspFMatrix4 *local, const ScePspFVector4 *s)
{
    ScePspFVector4 v = *s;
    ScePspFMatrix4 l = *local;
    ScePspFMatrix4 p = *parent;
    ScePspFMatrix4 m;
    ScePspFMatrix4 r;

    VfDot4x4(&m.w.x, &l.x.x, &l.y.x, &l.z.x, &l.w.x, v.x, v.y, v.z, 1.0f);
    m.x.x = VfMul(l.x.x, v.w);
    m.x.y = VfMul(l.x.y, v.w);
    m.x.z = VfMul(l.x.z, v.w);
    m.x.w = VfMul(l.x.w, v.w);
    m.y.x = VfMul(l.y.x, v.w);
    m.y.y = VfMul(l.y.y, v.w);
    m.y.z = VfMul(l.y.z, v.w);
    m.y.w = VfMul(l.y.w, v.w);
    m.z.x = VfMul(l.z.x, v.w);
    m.z.y = VfMul(l.z.y, v.w);
    m.z.z = VfMul(l.z.z, v.w);
    m.z.w = VfMul(l.z.w, v.w);

    r.x = ComposeField(&p, m.x.x, m.x.y, m.x.z, m.x.w);
    r.y = ComposeField(&p, m.y.x, m.y.y, m.y.z, m.y.w);
    r.z = ComposeField(&p, m.z.x, m.z.y, m.z.z, m.z.w);
    r.w = ComposeField(&p, m.w.x, m.w.y, m.w.z, m.w.w);
    *dst = r;
    return dst;
}
