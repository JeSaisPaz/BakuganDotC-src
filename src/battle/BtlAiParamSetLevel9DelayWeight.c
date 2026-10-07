// bdc 0x08a2a8e8 BtlAiParamSetLevel9DelayWeight
#include "bdc.h"

/* Level-9 entry of the `BtlAiParamSet` delay weight table (vtable `0x08af62c8` slot `+0x54`):
   returns the weight of delay index `delayIndex` for a CPU AI at skill level 9: 0 for indices
   1..5, 100 for every other index (index 0, a 0/30 s delay). */
s32 BtlAiParamSetLevel9DelayWeight(BtlAiParamSet *self, s32 delayIndex)
{
    (void)self;
    if ((u32)(delayIndex - 1) < 5) {
        return 0;
    }
    return 100;
}
