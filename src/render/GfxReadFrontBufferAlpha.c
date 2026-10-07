// bdc 0x089f2b5c GfxReadFrontBufferAlpha
#include "bdc.h"

/* Returns the alpha byte of pixel (`x`, `y`) of the frame buffer displayed last frame (0 outside
   480x272). `GfxLensFlareTaskUpdate` uses it to test whether the light source is occluded. */

u32 GfxReadFrontBufferAlpha(s32 x, s32 y)
{
  u8 *edram = (u8 *)sceGeEdramGetAddr();
  if (x >= 0 && x < 0x1e0 && y >= 0 && y < 0x110) {
    u32 *px = (u32 *)(edram + (g_gfxFrameIndex ^ 1) * 0x88000 + (x + y * 0x200) * 4);
    return *px >> 0x18;
  }
  return 0;
}
