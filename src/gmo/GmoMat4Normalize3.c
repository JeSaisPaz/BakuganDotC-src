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
    float rx = VfRsq(x.x * x.x + x.y * x.y + x.z * x.z);
    float ry = VfRsq(y.x * y.x + y.y * y.y + y.z * y.z);
    float rz = VfRsq(z.x * z.x + z.y * z.y + z.z * z.z);

    dst->x.x = x.x * rx;
    dst->x.y = x.y * rx;
    dst->x.z = x.z * rx;
    dst->x.w = x.w * rx;
    dst->y.x = y.x * ry;
    dst->y.y = y.y * ry;
    dst->y.z = y.z * ry;
    dst->y.w = y.w * ry;
    dst->z.x = z.x * rz;
    dst->z.y = z.y * rz;
    dst->z.z = z.z * rz;
    dst->z.w = z.w * rz;
    dst->w.x = 0.0f;
    dst->w.y = 0.0f;
    dst->w.z = 0.0f;
    dst->w.w = 1.0f;
    return dst;
}
