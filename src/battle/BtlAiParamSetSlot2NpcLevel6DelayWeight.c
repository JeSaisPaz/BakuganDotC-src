// bdc 0x08a2be00 BtlAiParamSetSlot2NpcLevel6DelayWeight
#include "bdc.h"

/* Level-6 entry of the `BtlAiParamSetSlot2Npc` delay weight table (vtable `0x08af6d50` slot
   `+0x3c`): returns the weight of delay index `delayIndex` for a CPU AI at skill level 6 — 0 for indices 1..5, 100 for any other
   index (index 0, a 0/30 s delay, in practice). */
s32 BtlAiParamSetSlot2NpcLevel6DelayWeight(BtlAiParamSet *self, s32 delayIndex)
{
    (void)self;
    if (delayIndex >= 1 && delayIndex <= 5) {
        return 0;
    }
    return 100;
}
