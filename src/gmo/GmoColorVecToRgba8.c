// bdc 0x08a13fa8 GmoColorVecToRgba8
#include "bdc.h"

/* Packs a float vec4 colour (each lane clamped to [0, 1], scaled by 255, truncated) into an RGBA8 word at
   `out`, lane 0 in the low byte; returns `out`. */

u32 *GmoColorVecToRgba8(u32 *out, const float *in)
{
  u32 packed = 0;
  int i;

  for (i = 0; i < 4; i++) {
    /* vsat0.q, vscl.q by 255 (viim), vf2iz.q ..., 23, vi2uc.q */
    packed |= (u32)VfI2uc(VfF2iz(VfSat0(in[i]) * 255.0f, 23)) << (i * 8);
  }
  *out = packed;
  return out;
}
