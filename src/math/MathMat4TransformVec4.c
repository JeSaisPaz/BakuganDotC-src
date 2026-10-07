// bdc 0x08a2977c MathMat4TransformVec4
#include "bdc.h"

/* Transforms the 4-vector `v` by the column-major 4x4 matrix `m` (`vtfm4.q` with `E100`):
   out[i] = sum over k of v[k] * column k of m, lane i. All inputs are read before `out` is written,
   so `out` may alias `v`. */
void MathMat4TransformVec4(const float *m, float *out, const float *v)
{
    float r[4];
    int i;

    for (i = 0; i < 4; i++) {
        r[i] = m[0 * 4 + i] * v[0] + m[1 * 4 + i] * v[1] + m[2 * 4 + i] * v[2] + m[3 * 4 + i] * v[3];
    }
    for (i = 0; i < 4; i++) {
        out[i] = r[i];
    }
}
