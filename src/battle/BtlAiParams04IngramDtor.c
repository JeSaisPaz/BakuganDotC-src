// bdc 0x08a2ad24 BtlAiParams04IngramDtor
#include "bdc.h"

/* Byte-identical compiled copy of `BtlAiParamsDtor` for `BtlAiParams04Ingram` (same instructions
   up to relocated addresses). */
void BtlAiParams04IngramDtor(BtlAiParams *self, u32 flags)
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
