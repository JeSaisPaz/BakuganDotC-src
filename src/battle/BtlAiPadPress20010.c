// bdc 0x08899508 BtlAiPadPress20010
#include "bdc.h"

/* AI virtual-pad action (like `BtlAiPadPress08`): ORs action bits 0x20010 into the pending
   action mask of the virtual pad and returns 1. */
s32 BtlAiPadPress20010(BtlAiPad *self)
{
    self->cur.aiActions |= 0x20010;
    return 1;
}
