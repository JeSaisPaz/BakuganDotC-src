// bdc 0x089dd594 GmoMat4Mul
#include "bdc.h"

/* Matrix product `dst = a * b` of two column-major 4x4 float matrices (fields are columns):
   `vmmul.q E000, E200, E100` in the assembler's operand convention (`vmmul.q Md, Ms, Mt` = `Ms · Mt`,
   as pspgum's `sceGumMultMatrix` uses it) gives `M000ᵀ = M200ᵀ · M100ᵀ = (M100 · M200)ᵀ`, so
   `dst = A · B`: column `j` of `dst` is `dst.j = Σ_k b.j[k] · a.k`. Both inputs are read before
   `dst` is written, so `dst` may alias either. Returns `dst`. */
#define MUL_COL(d, J)                                                                       \
    do {                                                                                    \
        (d).x = VfDot4(a->x.x, a->y.x, a->z.x, a->w.x, b->J.x, b->J.y, b->J.z, b->J.w); \
        (d).y = VfDot4(a->x.y, a->y.y, a->z.y, a->w.y, b->J.x, b->J.y, b->J.z, b->J.w); \
        (d).z = VfDot4(a->x.z, a->y.z, a->z.z, a->w.z, b->J.x, b->J.y, b->J.z, b->J.w); \
        (d).w = VfDot4(a->x.w, a->y.w, a->z.w, a->w.w, b->J.x, b->J.y, b->J.z, b->J.w); \
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
