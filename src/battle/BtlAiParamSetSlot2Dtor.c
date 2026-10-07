// bdc 0x08a2b5e0 BtlAiParamSetSlot2Dtor
#include "bdc.h"

/* Destructor of the `BtlAiParamSetSlot2` object (byte-identical compiled copy of
   `BtlAiParamsDtor`): resets the vtable to the `BtlAiParamSet` base vtable
   `g_btlAiParamSetVtbl` and, when bit 0 of `flags` is set, frees the object under the heap lock. */
void BtlAiParamSetSlot2Dtor(BtlAiParamSet *self, u32 flags)
{
    if (self != NULL) {
        self->vtbl = g_btlAiParamSetVtbl;
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(self, NULL, 0);
            MemUnlock();
        }
    }
}
