// bdc 0x08a2b3b0 BtlAiParams21ScorpionDtor
#include "bdc.h"

/* Byte-identical compiled copy of `BtlAiParamsDtor` for `BtlAiParams21Scorpion`: re-installs the base vtable
   `g_btlAiParamsVtable` and frees the object when bit 0 of `flags` is set. */
void BtlAiParams21ScorpionDtor(BtlAiParams *self, u32 flags)
{
    if (self == NULL) {
        return;
    }
    self->vtbl = (const VtblEntry *)g_btlAiParamsVtable;
    if (flags & 1) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}
