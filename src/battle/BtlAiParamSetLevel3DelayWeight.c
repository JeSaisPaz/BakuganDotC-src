// bdc 0x08a2a72c BtlAiParamSetLevel3DelayWeight
#include "bdc.h"

/* Level-3 entry of the `BtlAiParamSet` delay weight table (vtable `0x08af62c8` slot `+0x24`):
   returns the weight of delay index `delayIndex` (0..5) for a CPU AI at skill level 3 — 100 for
   index 5 (a 5/30 s delay), 0 for every other index. */

s32 BtlAiParamSetLevel3DelayWeight(BtlAiParamSet *self, s32 delayIndex)
{
    (void)self;
    if (delayIndex == 5) {
        return 100;
    }
    return 0;
}
