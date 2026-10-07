// bdc 0x08899578 BtlAiPadRelease04Press10
#include "bdc.h"

/* Releases the charge bit 4 and sets the attack bit 0x10 (fires a charged attack) on the virtual
   pad of `BtlAi`; returns 1. */
s32 BtlAiPadRelease04Press10(BtlAiPad *self)
{
    self->cur.aiActions = (self->cur.aiActions & ~4u) | 0x10;
    return 1;
}
