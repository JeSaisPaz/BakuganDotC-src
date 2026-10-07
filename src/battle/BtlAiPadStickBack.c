// bdc 0x08899438 BtlAiPadStickBack
#include "bdc.h"

/* Pulls the stick back (`stickY = -1.0`, heading π) and sets move bit 1 on the virtual pad of
   `BtlAi` (two 0x70-byte input controllers as read by `BtlInputReadActions`,
   the current one and the previous frame's, owner unit and stick axes); returns 1. */
s32 BtlAiPadStickBack(BtlAiPad *self)
{
    u32 actions = self->cur.aiActions;
    self->stickY = -1.0f;
    self->cur.aiActions = actions | 1;
    return 1;
}
