// bdc 0x08a2ac6c BtlAiParams11ElicoDtor
#include "bdc.h"

/* Byte-identical compiled copy of `BtlAiParamsDtor` for `BtlAiParams11Elico` (same instructions
   up to relocated addresses). */
void BtlAiParams11ElicoDtor(BtlAiParams *self, u32 flags)
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
