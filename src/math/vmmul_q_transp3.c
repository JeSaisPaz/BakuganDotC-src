// bdc 0x08a13c7c vmmul_q_transp3
#include "bdc.h"

/* One column of the product: b0 * a->x + b1 * a->y + b2 * a->z + b3 * a->w, lane by lane,
   summed left to right (the VFPU's dot-product order). */
static ScePspFVector4 MulColumn(const ScePspFMatrix4 *a, float b0, float b1, float b2, float b3)
{
    ScePspFVector4 r;

    r.x = b0 * a->x.x + b1 * a->y.x + b2 * a->z.x + b3 * a->w.x;
    r.y = b0 * a->x.y + b1 * a->y.y + b2 * a->z.y + b3 * a->w.y;
    r.z = b0 * a->x.z + b1 * a->y.z + b2 * a->z.z + b3 * a->w.z;
    r.w = b0 * a->x.w + b1 * a->y.w + b2 * a->z.w + b3 * a->w.w;
    return r;
}

/* VFPU product `vmmul.q E000, E200, E100` with `a` in M100 and `b` in M200 (fields as columns):
   `dst.<c> = Σ_k b.<c>[k] · a.<k>`, i.e. column-major `a · b` (row-major `b · a`). Both sources are read in full
   before `dst` is written, so `dst` may alias `a` or `b`. Also writes `dst->z.w` to `*zw` (S023, which
   the caller reads back from the VFPU). Returns `dst`. */
ScePspFMatrix4 *vmmul_q_transp3(ScePspFMatrix4 *dst, const ScePspFMatrix4 *a, const ScePspFMatrix4 *b,
                                float *zw)
{
    ScePspFMatrix4 r;

    r.x = MulColumn(a, b->x.x, b->x.y, b->x.z, b->x.w);
    r.y = MulColumn(a, b->y.x, b->y.y, b->y.z, b->y.w);
    r.z = MulColumn(a, b->z.x, b->z.y, b->z.z, b->z.w);
    r.w = MulColumn(a, b->w.x, b->w.y, b->w.z, b->w.w);
    *dst = r;
    *zw = r.z.w;
    return dst;
}
