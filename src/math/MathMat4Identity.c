// bdc 0x08a29700 MathMat4Identity
#include "bdc.h"

/* Sets the 4x4 matrix `m` to identity (`vmidt.q`, stored column by column). */
void MathMat4Identity(float *m)
{
    int c;
    int r;

    for (c = 0; c < 4; c++) {
        for (r = 0; r < 4; r++) {
            m[c * 4 + r] = (c == r) ? 1.0f : 0.0f;
        }
    }
}
