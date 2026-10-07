// bdc 0x08a13bd0 GmoVec3Set
#include "bdc.h"

/* Stores `(x, y, z)` into `out` and returns it. */

float *GmoVec3Set(float x, float y, float z, float *out)

{
  *out = x;
  out[1] = y;
  out[2] = z;
  return out;
}

