// bdc 0x089dffa4 GfxModelSetMotionLoop
#include "bdc.h"

/* Sets the loop flag (`slot+0xe`, 1 only when `loop == 1`) of the current motion slot (0x30-byte
   record `player+0x14 + model+0x134 * 0x30`, player = `model+0x130`). */

void GfxModelSetMotionLoop(GfxModel *self, s32 loop)
{
    GmoMotionRecord *rec = (GmoMotionRecord *)self->data->motions + self->motionSlot;
    rec->loop = (u16)((loop & 0xff) == 1);
}
