// bdc 0x08a13e34 GmoColorToRgba8
#include "bdc.h"

/* Packs four float components (each clamped to [0, 1] by vsat0, scaled by 255, then vf2iz 23 +
   vi2uc: floor clamped to [0, 255]) into an RGBA8 word at `out`, `r` in the low byte. Returns `out`. */

u32 *GmoColorToRgba8(float r, float g, float b, float a, u32 *out)

{
  *out = (u32)VfI2uc(VfF2iz(VfSat0(r) * 255.0f, 23)) |
         (u32)VfI2uc(VfF2iz(VfSat0(g) * 255.0f, 23)) << 8 |
         (u32)VfI2uc(VfF2iz(VfSat0(b) * 255.0f, 23)) << 16 |
         (u32)VfI2uc(VfF2iz(VfSat0(a) * 255.0f, 23)) << 24;
  return out;
}
