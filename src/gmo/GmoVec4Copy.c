// bdc 0x08a13e90 GmoVec4Copy
#include "bdc.h"

/* Copies a vec4. */

float *GmoVec4Copy(float *out, const float *in)

{
  out[0] = in[0];
  out[1] = in[1];
  out[2] = in[2];
  out[3] = in[3];
  return out;
}

