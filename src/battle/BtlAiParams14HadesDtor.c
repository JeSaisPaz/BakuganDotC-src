// bdc 0x08a2ade4 BtlAiParams14HadesDtor
#include "bdc.h"

/* Byte-identical compiled copy of `BtlAiParamsDtor` for `BtlAiParams14Hades` (same instructions
   up to relocated addresses). */

void BtlAiParams14HadesDtor(BtlAiParams *self, u32 flags)

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
