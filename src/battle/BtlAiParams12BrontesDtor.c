// bdc 0x08a2aaa0 BtlAiParams12BrontesDtor
#include "bdc.h"

/* Byte-identical compiled copy of `BtlAiParamsDtor` for `BtlAiParams12Brontes`: re-installs the base
   vtable `g_btlAiParamsVtable` and frees the object when bit 0 of `flags` is set. */
void BtlAiParams12BrontesDtor(BtlAiParams *self, u32 flags)
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
