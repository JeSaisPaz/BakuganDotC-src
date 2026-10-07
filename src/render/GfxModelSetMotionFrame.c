// bdc 0x089dff70 GfxModelSetMotionFrame
#include "bdc.h"

/* Sets the current frame of the current motion slot (0x30-byte record `player+0x14 + model+0x134 *
   0x30`, player = `model+0x130`) (`GmoMotionSetUser`). */

void GfxModelSetMotionFrame(GfxModel *self, float frame)

{
  GmoMotionSetUser((GmoMotionRecord *)((u8 *)self->data->motions + (uint)self->motionSlot * 0x30),frame);
}
