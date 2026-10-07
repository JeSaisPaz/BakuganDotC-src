// bdc 0x08a2bbac BtlAiParamSetSlot1NpcLevel9DelayWeight
#include "bdc.h"

/* Level-9 entry of the `BtlAiParamSetSlot1Npc` delay weight table (vtable `0x08af6cf0` slot
   `+0x54`): returns the weight of delay index `delayIndex` (0..5) for a CPU AI at skill level 9 —
   100 for index 0 (a 0/30 s delay), 0 for every other index. */
s32 BtlAiParamSetSlot1NpcLevel9DelayWeight(BtlAiParamSet *self, s32 delayIndex)
{
    (void)self;
    if ((u32)(delayIndex - 1) < 5) {
        return 0;
    }
    return 100;
}
