// bdc 0x089df420 GfxModelSwapMotionFrame
#include "bdc.h"

/* Sets the frame of the current motion slot (0x30-byte record `player+0x14 + model+0x134 * 0x30`,
   player = `model+0x130`) and returns the previous frame (`GfxModelGetMotionFrame`,
   `GfxModelSetMotionFrame`). */

float GfxModelSwapMotionFrame(GfxModel *self, float frame)

{
  float old;

  old = GfxModelGetMotionFrame(self);
  GfxModelSetMotionFrame(self, frame);
  return old;
}
