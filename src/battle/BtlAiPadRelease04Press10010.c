// bdc 0x08899594 BtlAiPadRelease04Press10010
#include "bdc.h"

/* Releases the charge bit 4 and sets bits 0x10010 (attack plus 0x10000) on the virtual pad of
   `BtlAi`; returns 1. */
s32 BtlAiPadRelease04Press10010(BtlAiPad *self)
{
    self->cur.aiActions = (self->cur.aiActions & 0xfffffffb) | 0x10010;
    return 1;
}
