// bdc 0x088995b8 BtlAiPadPressB0
#include "bdc.h"

/* AI virtual-pad action (like `BtlAiPadPress08`): ORs action bits 0xb0 into the pending action
   mask of the virtual pad and returns 1. */
s32 BtlAiPadPressB0(BtlAiPad *self)
{
    self->cur.aiActions |= 0xb0;
    return 1;
}
