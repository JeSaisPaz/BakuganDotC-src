// bdc 0x088995cc BtlAiPadPress130
#include "bdc.h"

/* AI virtual-pad action (like `BtlAiPadPress08`): ORs action bits 0x130 into the pending mask
   of the virtual pad and returns 1. */
s32 BtlAiPadPress130(BtlAiPad *self)
{
    self->cur.aiActions |= 0x130;
    return 1;
}
