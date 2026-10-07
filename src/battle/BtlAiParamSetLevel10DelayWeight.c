// bdc 0x08a2a930 BtlAiParamSetLevel10DelayWeight
#include "bdc.h"

/* Level-10 entry of the `BtlAiParamSet` delay weight table (vtable `0x08af62c8` slot `+0x5c`):
   returns the weight of delay index `delayIndex` (0..5) for a CPU AI at skill level 10 — 100 for
   index 0 (a 0/30 s delay), 0 for every other index. */

s32 BtlAiParamSetLevel10DelayWeight(BtlAiParamSet *self, s32 delayIndex)
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
