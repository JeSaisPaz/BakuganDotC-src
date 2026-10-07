// bdc 0x08899478 BtlAiPadStickDir4000
#include "bdc.h"

/* Pushes the stick sideways (stickX = 1.0, heading -pi/2, the relative direction 0x4000 of
   `BtlAiRaycastDir8`) and sets move bit 1 on the virtual pad of `BtlAi`;
   returns 1. */
s32 BtlAiPadStickDir4000(BtlAiPad *self)
{
    u32 actions = self->cur.aiActions;
    self->stickX = 1.0f;
    self->cur.aiActions = actions | 1;
    return 1;
}
