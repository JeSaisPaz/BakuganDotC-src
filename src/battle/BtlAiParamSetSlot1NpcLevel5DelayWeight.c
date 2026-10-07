// bdc 0x08a2ba80 BtlAiParamSetSlot1NpcLevel5DelayWeight
#include "bdc.h"

/* Level-5 entry of the `BtlAiParamSetSlot1Npc` delay weight table (vtable slot `+0x34`): returns
   the weight of delay index `delayIndex` for a CPU AI at skill level 5 — 100 for index 2 (a 2/30 s
   delay), 0 for every other index. */
s32 BtlAiParamSetSlot1NpcLevel5DelayWeight(BtlAiParamSet *self, s32 delayIndex)
{
    (void)self;
    if (delayIndex == 2) {
        return 100;
    }
    return 0;
}
