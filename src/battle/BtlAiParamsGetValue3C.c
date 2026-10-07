// bdc 0x08a2a610 BtlAiParamsGetValue3C
#include "bdc.h"

/* Default implementation of the per-species AI parameter getter at vtable offset `+0x3c` (base
   vtable `0x08af6280`, see `BtlAiCreateKindParams`): returns 0.1 for every level index 0–9 (the
   compiled switch has a single destination). */

float BtlAiParamsGetValue3C(BtlAiParams *self, s32 levelIndex)
{
  (void)self;
  (void)levelIndex;
  return 0.1f;
}
