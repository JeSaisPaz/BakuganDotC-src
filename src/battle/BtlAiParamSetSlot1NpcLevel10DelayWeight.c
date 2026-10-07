// bdc 0x08a2bbf4 BtlAiParamSetSlot1NpcLevel10DelayWeight
#include "bdc.h"

/* Level-10 entry of the `BtlAiParamSetSlot1Npc` delay weight table (vtable `0x08af6cf0` slot
   `+0x5c`): returns the weight of delay index `delayIndex` for a CPU AI at skill level 10
   — 0 for indices 1..5, 100 for any other
   index (index 0, a 0/30 s delay, in practice). */
s32 BtlAiParamSetSlot1NpcLevel10DelayWeight(BtlAiParamSet *self, s32 delayIndex)
{
    (void)self;
    if (delayIndex >= 1 && delayIndex <= 5) {
        return 0;
    }
    return 100;
}
