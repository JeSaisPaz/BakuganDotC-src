// bdc 0x088994a0 BtlAiPadPressDash
#include "bdc.h"

/* Presses the dash input on the AI virtual pad: action bit 2 when the owner's stat table dash mode
   is 1, else bits 0x202 (only 0x200 while the owner is in state 0xd); returns 1. */
s32 BtlAiPadPressDash(BtlAiPad *self)
{
    u32 actions = self->cur.aiActions;
    if (self->owner->combat.stats->dashMode == 1) {
        self->cur.aiActions = actions | 2;
        return 1;
    }
    if (self->owner->state != 0xd) {
        self->cur.aiActions = actions | 0x202;
        return 1;
    }
    self->cur.aiActions = actions | 0x200;
    return 1;
}
