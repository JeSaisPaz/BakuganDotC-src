// bdc 0x08a2a648 BtlAiParamSetDtor
#include "bdc.h"

/* Destructor of `BtlAiParamSet` (same code as `BtlAiParamsDtor`): restores the base vtable
   `g_btlAiParamSetVtbl` and frees the object when bit 0 of `flags` is set. */
void BtlAiParamSetDtor(BtlAiParamSet *self, u32 flags)
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
