// bdc 0x08a2bed8 BtlAiParamSetSlot2NpcLevel9DelayWeight
#include "bdc.h"

/* Level-9 delay weight of the battle AI parameter set `BtlAiParamSetSlot2Npc` (vtable `0x08af6d50`
   entry 10): returns 0 for `delayIndex` 1..5 and 100 otherwise. */
s32 BtlAiParamSetSlot2NpcLevel9DelayWeight(BtlAiParamSet *self, s32 delayIndex)
{
    (void)self;
    if ((u32)(delayIndex - 1) < 5) {
        return 0;
    }
    return 100;
}
