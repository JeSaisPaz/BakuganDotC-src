// bdc 0x08a13c4c GmoVec4SetB
#include "bdc.h"

/* Second compiled copy of `GmoVec4Set` (stores four words into `out`). */

float *GmoVec4SetB(float x, float y, float z, float w, float *out)

{
  *out = x;
  out[1] = y;
  out[2] = z;
  out[3] = w;
  return out;
}

