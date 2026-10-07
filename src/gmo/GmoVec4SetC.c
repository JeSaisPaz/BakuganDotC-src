// bdc 0x08a13c64 GmoVec4SetC
#include "bdc.h"

/* Third compiled copy of `GmoVec4Set` (stores four words into `out`). */

float *GmoVec4SetC(float x, float y, float z, float w, float *out)

{
  *out = x;
  out[1] = y;
  out[2] = z;
  out[3] = w;
  return out;
}

