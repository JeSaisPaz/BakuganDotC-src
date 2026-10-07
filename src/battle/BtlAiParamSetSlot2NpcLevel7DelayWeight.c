// bdc 0x08a2be48 BtlAiParamSetSlot2NpcLevel7DelayWeight
#include "bdc.h"

/* Level-7 entry of the `BtlAiParamSetSlot2Npc` delay weight table (vtable `0x08af6d50` slot
   `+0x44`): returns the weight of delay index `delayIndex` (0..5) for a CPU AI at skill level 7 —
   100 for index 0 (a 0/30 s delay), 0 for every other index. */
s32 BtlAiParamSetSlot2NpcLevel7DelayWeight(BtlAiParamSet *self, s32 delayIndex)
{
    (void)self;
    if ((u32)(delayIndex - 1) < 5) {
        return 0;
    }
    return 100;
}
