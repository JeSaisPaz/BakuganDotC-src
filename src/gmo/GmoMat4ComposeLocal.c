// bdc 0x089dd64c GmoMat4ComposeLocal
#include "bdc.h"

/* One field of the product: c0 * p->x + c1 * p->y + c2 * p->z + c3 * p->w, lane by lane, summed
   left to right. */
static ScePspFVector4 ComposeField(const ScePspFMatrix4 *p, float c0, float c1, float c2, float c3)
{
    ScePspFVector4 r;

    r.x = c0 * p->x.x + c1 * p->y.x + c2 * p->z.x + c3 * p->w.x;
    r.y = c0 * p->x.y + c1 * p->y.y + c2 * p->z.y + c3 * p->w.y;
    r.z = c0 * p->x.z + c1 * p->y.z + c2 * p->z.z + c3 * p->w.z;
    r.w = c0 * p->x.w + c1 * p->y.w + c2 * p->z.w + c3 * p->w.w;
    return r;
}

/* Composes a node's local matrix with a scale/offset vector `s` and its parent: builds `L'` whose
   x/y/z fields are those of `local` scaled by `s->w` (`vscl.q`) and whose w field is `local`
   applied to the point `s.xyz` (`vhtfm4.q`, row-vector convention), then
   `dst.<c> = Σ_k L'.<c>[k] · parent.<k>` (`vmmul.q E000, E200, E100`), i.e. row-major `L' · parent`.
   All inputs are read before `dst` is written, so `dst` may alias them. Used for node world
   matrices and bone matrices. Returns `dst`. */
ScePspFMatrix4 *GmoMat4ComposeLocal(ScePspFMatrix4 *dst, const ScePspFMatrix4 *parent, const ScePspFMatrix4 *local, const ScePspFVector4 *s)
{
    ScePspFVector4 v = *s;
    ScePspFMatrix4 l = *local;
    ScePspFMatrix4 p = *parent;
    ScePspFMatrix4 m;
    ScePspFMatrix4 r;

    m.w.x = l.x.x * v.x + l.y.x * v.y + l.z.x * v.z + l.w.x;
    m.w.y = l.x.y * v.x + l.y.y * v.y + l.z.y * v.z + l.w.y;
    m.w.z = l.x.z * v.x + l.y.z * v.y + l.z.z * v.z + l.w.z;
    m.w.w = l.x.w * v.x + l.y.w * v.y + l.z.w * v.z + l.w.w;
    m.x.x = l.x.x * v.w;
    m.x.y = l.x.y * v.w;
    m.x.z = l.x.z * v.w;
    m.x.w = l.x.w * v.w;
    m.y.x = l.y.x * v.w;
    m.y.y = l.y.y * v.w;
    m.y.z = l.y.z * v.w;
    m.y.w = l.y.w * v.w;
    m.z.x = l.z.x * v.w;
    m.z.y = l.z.y * v.w;
    m.z.z = l.z.z * v.w;
    m.z.w = l.z.w * v.w;

    r.x = ComposeField(&p, m.x.x, m.x.y, m.x.z, m.x.w);
    r.y = ComposeField(&p, m.y.x, m.y.y, m.y.z, m.y.w);
    r.z = ComposeField(&p, m.z.x, m.z.y, m.z.z, m.z.w);
    r.w = ComposeField(&p, m.w.x, m.w.y, m.w.z, m.w.w);
    *dst = r;
    return dst;
}
