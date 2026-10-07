// bdc 0x08a13bc0 GmoVec2Set
#include "bdc.h"

/* Stores `(x, y)` into `out` and returns it. */

float *GmoVec2Set(float x, float y, float *out)

{
  *out = x;
  out[1] = y;
  return out;
}

