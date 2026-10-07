// bdc 0x08a2bb64 BtlAiParamSetSlot1NpcLevel8DelayWeight
#include "bdc.h"

/* Level-8 entry of the `BtlAiParamSetSlot1Npc` delay weight table: returns the weight of delay index
   `delayIndex` for a CPU AI at skill level 8 — 0 for indices 1..5, 100 for any other index
   (index 0, a 0/30 s delay, in practice). */
s32 BtlAiParamSetSlot1NpcLevel8DelayWeight(BtlAiParamSet *self, s32 delayIndex)
{
    (void)self;
    if ((u32)(delayIndex - 1) < 5) {
        return 0;
    }
    return 100;
}
