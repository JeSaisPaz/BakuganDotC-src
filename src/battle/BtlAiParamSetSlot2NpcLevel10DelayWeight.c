// bdc 0x08a2bf20 BtlAiParamSetSlot2NpcLevel10DelayWeight
#include "bdc.h"

/* Level-10 delay weight of the battle AI parameter set `BtlAiParamSetSlot2Npc` (vtable entry 11):
   returns 0 for `delayIndex` 1..5 and 100 otherwise. */
s32 BtlAiParamSetSlot2NpcLevel10DelayWeight(BtlAiParamSet *self, s32 delayIndex)
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
