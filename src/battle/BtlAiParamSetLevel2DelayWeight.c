// bdc 0x08a2a6e8 BtlAiParamSetLevel2DelayWeight
#include "bdc.h"

/* Level-2 entry of the `BtlAiParamSet` delay weight table (vtable `0x08af62c8` slot `+0x1c`):
   returns the weight of delay index `delayIndex` (0..5) for a CPU AI at skill level 2 — 100 for
   index 5 (a 5/30 s delay), 0 for every other index. */

s32 BtlAiParamSetLevel2DelayWeight(BtlAiParamSet *self, s32 delayIndex)

{
  (void)self;
  return delayIndex == 5 ? 100 : 0;
}
