// bdc 0x08a2b584 BtlAiParamSetSlot1Dtor
#include "bdc.h"

/* Deleting destructor of `BtlAiParamSetSlot1` (byte-identical to `BtlAiParamsDtor`): re-installs
   the base vtable `g_btlAiParamSetVtbl` and frees the object when bit 0 of `flags` is set. Does
   nothing for NULL. */
void BtlAiParamSetSlot1Dtor(BtlAiParamSet *self, u32 flags)
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
