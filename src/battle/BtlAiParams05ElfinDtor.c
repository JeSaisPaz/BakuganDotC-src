// bdc 0x08a2abb4 BtlAiParams05ElfinDtor
#include "bdc.h"

/* Byte-identical compiled copy of `BtlAiParamsDtor` for `BtlAiParams05Elfin`: re-installs the base
   vtable `g_btlAiParamsVtable` and frees the object when bit 0 of `flags` is set. */
void BtlAiParams05ElfinDtor(BtlAiParams *self, u32 flags)
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
