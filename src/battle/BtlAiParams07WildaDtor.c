// bdc 0x08a2b4cc BtlAiParams07WildaDtor
#include "bdc.h"

/* Byte-identical compiled copy of `BtlAiParamsDtor` for `BtlAiParams07Wilda` (same instructions
   up to relocated addresses). */
void BtlAiParams07WildaDtor(BtlAiParams *self, u32 flags)
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
