// bdc 0x08a2bce0 BtlAiParamSetSlot2NpcLevel2DelayWeight
#include "bdc.h"

/* Level-2 entry of the `BtlAiParamSetSlot2Npc` delay weight table (vtable `0x08af6d50` slot
   `+0x1c`): returns the weight of delay index `delayIndex` (0..5) for a CPU AI at skill level 2 —
   100 for index 0 (a 0/30 s delay), 0 for every other index. */
s32 BtlAiParamSetSlot2NpcLevel2DelayWeight(BtlAiParamSet *self, s32 delayIndex)
{
    (void)self;
    if ((u32)(delayIndex - 1) < 5) {
        return 0;
    }
    return 100;
}
