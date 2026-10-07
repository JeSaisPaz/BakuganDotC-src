// bdc 0x08a13dbc GmoLerpFloats
#include "bdc.h"

/* Linear interpolation of `n` floats: `out[i] = a[i] + t * (b[i] - a[i])` with `t` clamped to [0,
   1]. */

void GmoLerpFloats(float t, float *out, const float *a, const float *b, int n)
{
  int i;

  if (1.0f < t) {
    t = 1.0f;
  } else if (t < 0.0f) {
    t = 0.0f;
  }
  for (i = 0; i < n; i++) {
    out[i] = a[i] + t * (b[i] - a[i]);
  }
}
