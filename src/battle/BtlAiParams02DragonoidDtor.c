// bdc 0x08a2afb0 BtlAiParams02DragonoidDtor
#include "bdc.h"

/* Byte-identical compiled copy of `BtlAiParamsDtor` for `BtlAiParams02Dragonoid` (same
   instructions up to relocated addresses). */
void BtlAiParams02DragonoidDtor(BtlAiParams *self, u32 flags)
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
