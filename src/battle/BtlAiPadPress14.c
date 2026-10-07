// bdc 0x08899538 BtlAiPadPress14
#include "bdc.h"

/* Sets action bits 0x14 (charge hold plus attack) on the current frame of the AI virtual pad;
   returns 1. */
s32 BtlAiPadPress14(BtlAiPad *self)
{
    self->cur.aiActions |= 0x14;
    return 1;
}
