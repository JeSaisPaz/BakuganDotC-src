// bdc 0x088995e0 BtlAiPadPress08
#include "bdc.h"

/* Sets action bit 8 on the current input of the AI virtual pad; returns 1. */
s32 BtlAiPadPress08(BtlAiPad *self)
{
    self->cur.aiActions |= 8;
    return 1;
}
