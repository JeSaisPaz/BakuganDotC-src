// bdc 0x08a2ab58 BtlAiParams29DynamoDtor
#include "bdc.h"

/* Byte-identical compiled copy of `BtlAiParamsDtor` for `BtlAiParams29Dynamo`: re-installs the base
   vtable `g_btlAiParamsVtable` and frees the object when bit 0 of `flags` is set. */
void BtlAiParams29DynamoDtor(BtlAiParams *self, u32 flags)
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
