// bdc 0x08a2acc8 BtlAiParams24FalconflyDtor
#include "bdc.h"

/* Byte-identical compiled copy of `BtlAiParamsDtor` for `BtlAiParams24Falconfly` (same
   instructions up to relocated addresses). */
void BtlAiParams24FalconflyDtor(BtlAiParams *self, u32 flags)
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
