// bdc 0x08a2bd70 BtlAiParamSetSlot2NpcLevel4DelayWeight
#include "bdc.h"

/* Level-4 entry of the `BtlAiParamSetSlot2Npc` delay weight table (vtable `0x08af6d50` slot
   `+0x2c`): returns the weight of delay index `delayIndex` for a CPU AI at skill level 4 — 0 for indices 1..5, 100 for any other
   index (index 0, a 0/30 s delay, in practice). */
s32 BtlAiParamSetSlot2NpcLevel4DelayWeight(BtlAiParamSet *self, s32 delayIndex)
{
    (void)self;
    if (delayIndex >= 1 && delayIndex <= 5) {
        return 0;
    }
    return 100;
}
