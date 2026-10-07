// bdc 0x089dd594 GmoMat4Mul
#include "bdc.h"

/* Matrix product `dst = a * b` of two column-major 4x4 float matrices (fields are columns):
   `vmmul.q E000, E200, E100` in the assembler's operand convention (`vmmul.q Md, Ms, Mt` = `Ms · Mt`,
   as pspgum's `sceGumMultMatrix` uses it) gives `M000ᵀ = M200ᵀ · M100ᵀ = (M100 · M200)ᵀ`, so
   `dst = A · B`: column `j` of `dst` is `dst.j = Σ_k b.j[k] · a.k`. Both inputs are read before
   `dst` is written, so `dst` may alias either. Returns `dst`. */
#define MUL_COL(d, J)                                                                       \
    do {                                                                                    \
        (d).x = a->x.x * b->J.x + a->y.x * b->J.y + a->z.x * b->J.z + a->w.x * b->J.w;      \
        (d).y = a->x.y * b->J.x + a->y.y * b->J.y + a->z.y * b->J.z + a->w.y * b->J.w;      \
        (d).z = a->x.z * b->J.x + a->y.z * b->J.y + a->z.z * b->J.z + a->w.z * b->J.w;      \
        (d).w = a->x.w * b->J.x + a->y.w * b->J.y + a->z.w * b->J.z + a->w.w * b->J.w;      \
    } while (0)

ScePspFMatrix4 *GmoMat4Mul(ScePspFMatrix4 *dst, const ScePspFMatrix4 *a, const ScePspFMatrix4 *b)
{
    ScePspFMatrix4 r;

    MUL_COL(r.x, x);
    MUL_COL(r.y, y);
    MUL_COL(r.z, z);
    MUL_COL(r.w, w);
    *dst = r;
    return dst;
}
