// bdc 0x08a2aef8 BtlAiParams03IngramDtor
#include "bdc.h"

/* Byte-identical compiled copy of `BtlAiParamsDtor` for `BtlAiParams03Ingram` (same instructions
   up to relocated addresses). */

void BtlAiParams03IngramDtor(BtlAiParams *self, u32 flags)

{
  if (self != NULL) {
    self->vtbl = (VtblEntry *)g_btlAiParamsVtable;
    if (flags & 1) {
      MemLock();
      MemFree(self, NULL, 0);
      MemUnlock();
    }
  }
}
