// bdc 0x08a2af54 BtlAiParams19WildaDtor
#include "bdc.h"

/* Byte-identical compiled copy of `BtlAiParamsDtor` for `BtlAiParams19Wilda` (same instructions
   up to relocated addresses). */

void BtlAiParams19WildaDtor(BtlAiParams *self, u32 flags)

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
