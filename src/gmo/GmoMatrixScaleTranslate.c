// bdc 0x08a13bfc GmoMatrixScaleTranslate
#include "bdc.h"

/* Builds the 4x4 matrix `out` with diagonal `(sx, sy, sz, 1)` and translation `(tx, ty, tz)` (row
   3), all other terms 0. Returns `out`. */

float *GmoMatrixScaleTranslate(float tx, float ty, float tz, float sx, float sy, float sz, float *out)
{
  out[0] = sx;
  out[5] = sy;
  out[10] = sz;
  out[12] = tx;
  out[13] = ty;
  out[14] = tz;
  out[15] = 1.0f;
  out[1] = 0.0f;
  out[2] = 0.0f;
  out[3] = 0.0f;
  out[4] = 0.0f;
  out[6] = 0.0f;
  out[7] = 0.0f;
  out[8] = 0.0f;
  out[9] = 0.0f;
  out[11] = 0.0f;
  return out;
}
