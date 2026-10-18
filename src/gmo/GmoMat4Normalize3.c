// bdc 0x089dd69c GmoMat4Normalize3
#include "bdc.h"

/* Scales each of the first three fields of `m` (all four lanes) by the inverse length of its xyz
   part (`vdot.t` + `vrsq.t`), removing scale, and sets the translation field to (0, 0, 0, 1)
   (`vidt.q C030`). `m` is read in full before `dst` is written, so they may alias. Returns `dst`. */
ScePspFMatrix4 *GmoMat4Normalize3(ScePspFMatrix4 *dst, const ScePspFMatrix4 *m)
{
    ScePspFVector4 x = m->x;
    ScePspFVector4 y = m->y;
    ScePspFVector4 z = m->z;
    float rx = VfRsq(VfDot3(x.x, x.y, x.z, x.x, x.y, x.z));
    float ry = VfRsq(VfDot3(y.x, y.y, y.z, y.x, y.y, y.z));
    float rz = VfRsq(VfDot3(z.x, z.y, z.z, z.x, z.y, z.z));

    dst->x.x = VfMul(x.x, rx);
    dst->x.y = VfMul(x.y, rx);
    dst->x.z = VfMul(x.z, rx);
    dst->x.w = VfMul(x.w, rx);
    dst->y.x = VfMul(y.x, ry);
    dst->y.y = VfMul(y.y, ry);
    dst->y.z = VfMul(y.z, ry);
    dst->y.w = VfMul(y.w, ry);
    dst->z.x = VfMul(z.x, rz);
    dst->z.y = VfMul(z.y, rz);
    dst->z.z = VfMul(z.z, rz);
    dst->z.w = VfMul(z.w, rz);
    dst->w.x = 0.0f;
    dst->w.y = 0.0f;
    dst->w.z = 0.0f;
    dst->w.w = 1.0f;
    return dst;
}
