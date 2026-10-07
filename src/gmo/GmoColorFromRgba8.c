// bdc 0x08a13da0 GmoColorFromRgba8
#include "bdc.h"

/* Expands a packed RGBA8 colour (`*in`, lane 0 in the low byte) to a float vec4 in `out`, each
   channel byte b mapped to about b/255 (vuc2i: (b * 0x01010101) >> 1, then vi2f by 2^31; 255 → 1.0f).
   Returns `out`. */

float *GmoColorFromRgba8(float *out, const u32 *in)

{
  u32 packed = *in;
  int i;

  for (i = 0; i < 4; i++) {
    u32 b = (packed >> (i * 8)) & 0xff;
    out[i] = (float)(int)(b * 0x01010101u >> 1) / 2147483648.0f;
  }
  return out;
}
