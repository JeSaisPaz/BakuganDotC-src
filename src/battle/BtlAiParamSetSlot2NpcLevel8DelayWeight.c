// bdc 0x08a2be90 BtlAiParamSetSlot2NpcLevel8DelayWeight
#include "bdc.h"

/* Level-8 delay weight of the battle AI parameter set `BtlAiParamSetSlot2Npc` (vtable `0x08af6d50`
   entry 9): returns 0 for `delayIndex` 1..5 and 100 otherwise. */
s32 BtlAiParamSetSlot2NpcLevel8DelayWeight(BtlAiParamSet *self, s32 delayIndex)
{
    (void)self;
    if (delayIndex >= 1 && delayIndex <= 5) {
        return 0;
    }
    return 100;
}
