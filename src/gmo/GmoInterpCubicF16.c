// bdc 0x089daca0 GmoInterpCubicF16
#include "bdc.h"

/* Half-float twin of `GmoInterpCubicF32` used by `GmoMotionEvalKeyF16`: converts the half
   values and handles of both keys (`vh2f`), then evaluates the same cubic Bezier at `t`: control
   points key0's value, `value + outHandle` of key0, `value + inHandle` of key1 and key1's value,
   weighted with the Bernstein basis of `t` and `u = 1 - t`. Returns the 4 interpolated components
   (left in VFPU C000 on the PSP). */

ScePspFVector4 GmoInterpCubicF16(float t, const GmoMotionKeyCubicH *key0, const GmoMotionKeyCubicH *key1)
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
        p0[i] = VfH2f(key0->comp[i].value);
        p3[i] = VfH2f(key1->comp[i].value);
        p1[i] = VfMul(VfAdd(VfH2f(key0->comp[i].outHandle), p0[i]), 3.0f);
        p2[i] = VfMul(VfAdd(VfH2f(key1->comp[i].inHandle), p3[i]), 3.0f);
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
