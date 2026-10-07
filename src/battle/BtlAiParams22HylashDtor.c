// bdc 0x08a2ae9c BtlAiParams22HylashDtor
#include "bdc.h"

/* Byte-identical compiled copy of `BtlAiParamsDtor` for `BtlAiParams22Hylash` (same instructions
   up to relocated addresses). */

void BtlAiParams22HylashDtor(BtlAiParams *self, u32 flags)

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
