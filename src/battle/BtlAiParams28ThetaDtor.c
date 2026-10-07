// bdc 0x08a2b470 BtlAiParams28ThetaDtor
#include "bdc.h"

/* Byte-identical compiled copy of `BtlAiParamsDtor` for `BtlAiParams28Theta` (same instructions
   up to relocated addresses). */
void BtlAiParams28ThetaDtor(BtlAiParams *self, u32 flags)
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
