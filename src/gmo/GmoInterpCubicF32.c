// bdc 0x089dac20 GmoInterpCubicF32
#include "bdc.h"

/* Cubic Bezier interpolation of a 4-component float animation key pair at `t` (mode 2 of
   `GmoMotionEvalKeyF32`): per component the control points are key0's value, `value + outHandle`
   of key0, `value + inHandle` of key1 and key1's value, weighted with the Bernstein basis of `t`
   and `u = 1 - t`. Returns the 4 interpolated components (left in VFPU C000 on the PSP). */

ScePspFVector4 GmoInterpCubicF32(float t, const GmoMotionKeyCubicF *key0, const GmoMotionKeyCubicF *key1)
{
    float p0[4];
    float p1[4];
    float p2[4];
    float p3[4];
    float w[4];
    float u;
    float u2;
    float t2;
    float r[4];
    ScePspFVector4 out;
    s32 i;

    for (i = 0; i < 4; i++) {
        p0[i] = key0->comp[i].value;
        p1[i] = VfMul(VfAdd(key0->comp[i].outHandle, key0->comp[i].value), 3.0f);
        p3[i] = key1->comp[i].value;
        p2[i] = VfMul(VfAdd(key1->comp[i].inHandle, key1->comp[i].value), 3.0f);
    }
    u = VfSub(1.0f, t);
    u2 = VfMul(u, u);
    t2 = VfMul(t, t);
    w[0] = VfMul(u, u2);
    w[1] = VfMul(t, t2);
    w[2] = VfMul(t, u2);
    w[3] = VfMul(u, t2);
    for (i = 0; i < 4; i++) {
        r[i] = VfDot4(p0[i], p3[i], p1[i], p2[i], w[0], w[1], w[2], w[3]);
    }
    out.x = r[0];
    out.y = r[1];
    out.z = r[2];
    out.w = r[3];
    return out;
}
