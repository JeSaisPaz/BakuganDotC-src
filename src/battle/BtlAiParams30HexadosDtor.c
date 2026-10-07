// bdc 0x08a2ae40 BtlAiParams30HexadosDtor
#include "bdc.h"

/* Byte-identical compiled copy of `BtlAiParamsDtor` for `BtlAiParams30Hexados` (same instructions
   up to relocated addresses). */

void BtlAiParams30HexadosDtor(BtlAiParams *self, u32 flags)

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
