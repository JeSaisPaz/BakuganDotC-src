// bdc 0x08a2b7ec BtlAiParamSetSlot2Level7DelayWeight
#include "bdc.h"

/* Level-7 entry of the `BtlAiParamSetSlot2` delay weight table (vtable `0x08af6c90` slot `+0x44`):
   returns the weight of delay index `delayIndex` for a CPU AI at skill level 7 — 0 for indices
   1..5, 100 for every other index (index 0, a 0/30 s delay, in practice). */
s32 BtlAiParamSetSlot2Level7DelayWeight(BtlAiParamSet *self, s32 delayIndex)
{
    (void)self;
    if (delayIndex >= 1 && delayIndex <= 5) {
        return 0;
    }
    return 100;
}
