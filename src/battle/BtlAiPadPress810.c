// bdc 0x08899524 BtlAiPadPress810
#include "bdc.h"

/* AI virtual-pad action (like `BtlAiPadPress08`): ORs action bits 0x810 into the pending action
   mask of the virtual pad and returns 1. */
s32 BtlAiPadPress810(BtlAiPad *self)
{
    self->cur.aiActions |= 0x810;
    return 1;
}
