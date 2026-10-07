// bdc 0x08a2a5a4 BtlAiParamsGetChance9E4
#include "bdc.h"

/* Default per-species AI parameter getter (entry 6, fn at `+0x34` of the base vtable
   `g_btlAiParamsVtable`): 80 for `level` 5..10, 0 for any other level (compiled as a jump table
   over 1..10). */
s32 BtlAiParamsGetChance9E4(BtlAiParams *self, s32 level)
{
  (void)self;
  if (level >= 5 && level <= 10) {
    return 80;
  }
  return 0;
}
