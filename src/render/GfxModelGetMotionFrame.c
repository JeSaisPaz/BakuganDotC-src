// bdc 0x089dff08 GfxModelGetMotionFrame
#include "bdc.h"

/* Returns the current frame of the current motion slot (0x30-byte record `player+0x14 + model+0x134
   * 0x30`, player = `model+0x130`) (`GmoMotionGetUser`). */

float GfxModelGetMotionFrame(GfxModel *self)

{
  return GmoMotionGetUser((GmoMotionRecord *)((u8 *)self->data->motions + (uint)self->motionSlot * 0x30));
}
