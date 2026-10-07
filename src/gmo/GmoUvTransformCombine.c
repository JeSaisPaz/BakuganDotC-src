// bdc 0x089dd6e0 GmoUvTransformCombine
#include "bdc.h"

/* Combines two UV transform records `{uOff, vOff, uScale, vScale}`: the result offsets are
   `b.off * a.scale + a.off`, the result scales are `b.scale * a.scale`. All inputs are read
   before `dst` is written, so `dst` may alias `a` or `b`. Returns `dst`. */
float *GmoUvTransformCombine(float *dst, const float *a, const float *b)
{
    float a0 = a[0];
    float a1 = a[1];
    float a2 = a[2];
    float a3 = a[3];
    float r0 = b[0] * a2 + a0;
    float r1 = b[1] * a3 + a1;
    float r2 = b[2] * a2;
    float r3 = b[3] * a3;

    dst[0] = r0;
    dst[1] = r1;
    dst[2] = r2;
    dst[3] = r3;
    return dst;
}
