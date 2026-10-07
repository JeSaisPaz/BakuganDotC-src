// bdc 0x08899458 BtlAiPadStickDirC000
#include "bdc.h"

/* Pushes the stick sideways (stickX = -1.0, heading +pi/2, the relative direction 0xc000 of
   `BtlAiRaycastDir8`) and sets move bit 1 on the virtual pad of `BtlAi`;
   returns 1. */
s32 BtlAiPadStickDirC000(BtlAiPad *self)
{
    u32 actions = self->cur.aiActions;

    self->stickX = -1.0f;
    self->cur.aiActions = actions | 1;
    return 1;
}
