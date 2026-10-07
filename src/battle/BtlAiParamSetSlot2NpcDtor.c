// bdc 0x08a2bc3c BtlAiParamSetSlot2NpcDtor
#include "bdc.h"

/* Byte-identical compiled copy of `BtlAiParamsDtor` for `BtlAiParamSetSlot2Npc`: re-installs the
   base vtable `g_btlAiParamSetVtbl` and frees the object when bit 0 of `flags` is set. */
void BtlAiParamSetSlot2NpcDtor(BtlAiParamSet *self, u32 flags)
{
    if (self == NULL) {
        return;
    }
    self->vtbl = g_btlAiParamSetVtbl;
    if (flags & 1) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}
