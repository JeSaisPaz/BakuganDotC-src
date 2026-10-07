// bdc 0x08a2a470 BtlAiParamsDtor
#include "bdc.h"

/* Deleting destructor of the per-species AI parameter base class (the 8-byte `{kind, vtable}`
   objects made by `BtlAiCreateKindParams`): re-installs the base vtable `g_btlAiParamsVtable`
   and frees the object when bit 0 of `flags` is set. Does nothing for NULL. */
void BtlAiParamsDtor(BtlAiParams *self, u32 flags)
{
    if (self != NULL) {
        self->vtbl = (const VtblEntry *)g_btlAiParamsVtable;
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(self, NULL, 0);
            MemUnlock();
        }
    }
}
