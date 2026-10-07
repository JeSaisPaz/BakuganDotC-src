// bdc 0x08a2b90c BtlAiParamSetSlot1NpcDtor
#include "bdc.h"

/* Deleting destructor of the slot-1 AI parameter set `BtlAiParamSetSlot1Npc` (the `ai+0x2d0`
   object built by `BtlAiCreateKindParamSet` for kinds 0x15..0x20; same code as
   `BtlAiParamSetDtor`): re-installs the `BtlAiParamSet` base vtable `g_btlAiParamSetVtbl` and
   frees the object when bit 0 of `flags` is set. Does nothing for NULL. */
void BtlAiParamSetSlot1NpcDtor(BtlAiParamSet *self, u32 flags)
{
    if (self != NULL) {
        self->vtbl = (const VtblEntry *)g_btlAiParamSetVtbl;
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(self, NULL, 0);
            MemUnlock();
        }
    }
}
