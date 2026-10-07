// bdc 0x08a2b87c BtlAiParamSetSlot2Level9DelayWeight
#include "bdc.h"

/* Level-9 entry of the `BtlAiParamSetSlot2` delay weight table: returns the weight of delay index
   `delayIndex` for a CPU AI at skill level 9 — 0 for indices 1..5, 100 for any other index
   (index 0, a 0/30 s delay, in practice). */
s32 BtlAiParamSetSlot2Level9DelayWeight(BtlAiParamSet *self, s32 delayIndex)
{
    (void)self;
    if ((u32)(delayIndex - 1) < 5) {
        return 0;
    }
    return 100;
}
