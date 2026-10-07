// bdc 0x08899418 BtlAiPadStickForward
#include "bdc.h"

/* Pushes the stick forward (`stickY = 1.0`) and sets move bit 1 on the current input of the AI
   virtual pad; returns 1. */
s32 BtlAiPadStickForward(BtlAiPad *self)
{
    u32 actions = self->cur.aiActions;
    self->stickY = 1.0f;
    self->cur.aiActions = actions | 1;
    return 1;
}
