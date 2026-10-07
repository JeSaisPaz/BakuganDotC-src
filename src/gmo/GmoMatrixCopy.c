// bdc 0x08a13eb8 GmoMatrixCopy
#include "bdc.h"

/* Copies a 4x4 matrix (16 floats) from `in` to `out`. Returns `out`. */

float *GmoMatrixCopy(float *out, const float *in)
{
  int i;

  for (i = 0; i < 16; i += 4) {
    float a = in[i + 1];
    float b = in[i + 2];
    float c = in[i + 3];
    float d = in[i];
    out[i + 1] = a;
    out[i + 2] = b;
    out[i + 3] = c;
    out[i] = d;
  }
  return out;
}
