// bdc 0x08a2b7a4 BtlAiParamSetSlot2Level6DelayWeight
#include "bdc.h"

/* Level-6 entry of the `BtlAiParamSetSlot2` delay weight table (vtable `0x08af6c90` slot `+0x3c`):
   returns the weight of delay index `delayIndex` (0..5) for a CPU AI at skill level 6 — 100 for
   index 0 (a 0/30 s delay), 0 for every other index. */

s32 BtlAiParamSetSlot2Level6DelayWeight(BtlAiParamSet *self, s32 delayIndex)
{
  (void)self;
  switch (delayIndex) {
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
    return 0;
  default:
    return 100;
  }
}
