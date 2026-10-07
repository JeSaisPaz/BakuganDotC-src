// bdc 0x08a13be4 GmoVec4Set
#include "bdc.h"

/* Stores `(x, y, z, w)` into `out` and returns it. */

float *GmoVec4Set(float x, float y, float z, float w, float *out)

{
  *out = x;
  out[1] = y;
  out[2] = z;
  out[3] = w;
  return out;
}

