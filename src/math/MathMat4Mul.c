// bdc 0x08a29624 MathMat4Mul
#include "bdc.h"

/* Column `col` of `a * b`: the sum over k of col[k] * column k of `a`. */
static void MathMat4MulColumn(ScePspFVector4 *d, const ScePspFMatrix4 *a, const ScePspFVector4 *col)
{
    d->x = col->x * a->x.x + col->y * a->y.x + col->z * a->z.x + col->w * a->w.x;
    d->y = col->x * a->x.y + col->y * a->y.y + col->z * a->z.y + col->w * a->w.y;
    d->z = col->x * a->x.z + col->y * a->y.z + col->z * a->z.z + col->w * a->w.z;
    d->w = col->x * a->x.w + col->y * a->y.w + col->z * a->z.w + col->w * a->w.w;
}

/* 4x4 matrix product with the VFPU (`vmmul.q M000, M100, M200`): `out = a * b` in the engine's
   column-major layout, column j of `out` = sum over k of b[j][k] * column k of `a`. Both inputs are
   read before `out` is written, so `out` may alias `a` or `b`. Returns `out`. */
ScePspFMatrix4 *MathMat4Mul(ScePspFMatrix4 *out, const ScePspFMatrix4 *a, const ScePspFMatrix4 *b)
{
    ScePspFMatrix4 r;

    MathMat4MulColumn(&r.x, a, &b->x);
    MathMat4MulColumn(&r.y, a, &b->y);
    MathMat4MulColumn(&r.z, a, &b->z);
    MathMat4MulColumn(&r.w, a, &b->w);
    *out = r;
    return out;
}
