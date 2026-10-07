// bdc 0x08a2b63c BtlAiParamSetSlot2Level1DelayWeight
#include "bdc.h"

/* Level-1 entry of the `BtlAiParamSetSlot2` delay weight table (vtable `0x08af6c90` slot `+0x14`):
   returns the weight of delay index `delayIndex` (0..5) for a CPU AI at skill level 1 — 0 for
   indices 1..5 and 100 for any other index (index 0 is the 0/30 s delay). */

s32 BtlAiParamSetSlot2Level1DelayWeight(BtlAiParamSet *self, s32 delayIndex)

{
  (void)self;
  if (delayIndex >= 1 && delayIndex <= 5) {
    return 0;
  }
  return 100;
}
