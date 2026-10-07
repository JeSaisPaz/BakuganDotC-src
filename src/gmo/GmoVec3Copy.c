// bdc 0x08a13e70 GmoVec3Copy
#include "bdc.h"

/* Copies a vec3 (`*out = *in`). */

float *GmoVec3Copy(float *out, const float *in)
{
  float x = in[0];
  float y = in[1];
  float z = in[2];
  out[0] = x;
  out[1] = y;
  out[2] = z;
  return out;
}
