// bdc 0x08a13f58 GmoVec4CopyB
#include "bdc.h"

/* Second compiled copy of `GmoVec4Copy`. */

float *GmoVec4CopyB(float *out, const float *in)

{
  out[0] = in[0];
  out[1] = in[1];
  out[2] = in[2];
  out[3] = in[3];
  return out;
}

