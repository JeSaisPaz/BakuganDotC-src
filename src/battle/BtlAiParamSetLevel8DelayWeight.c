// bdc 0x08a2a8a0 BtlAiParamSetLevel8DelayWeight
#include "bdc.h"

/* Level-8 entry of the `BtlAiParamSet` delay weight table (vtable `0x08af62c8` slot `+0x4c`):
   returns the weight of delay index `delayIndex` for a CPU AI at skill level 8 -- 0 for indices
   1..5, 100 for every other index (index 0 is the 0/30 s delay). */
s32 BtlAiParamSetLevel8DelayWeight(BtlAiParamSet *self, s32 delayIndex)
{
    (void)self;
    if ((u32)(delayIndex - 1) < 5) {
        return 0;
    }
    return 100;
}
