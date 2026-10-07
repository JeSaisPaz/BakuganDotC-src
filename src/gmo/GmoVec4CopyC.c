// bdc 0x08a13f80 GmoVec4CopyC
#include "bdc.h"

/* Third compiled copy of `GmoVec4Copy`. */

float *GmoVec4CopyC(float *out, const float *in)

{
  out[0] = in[0];
  out[1] = in[1];
  out[2] = in[2];
  out[3] = in[3];
  return out;
}

