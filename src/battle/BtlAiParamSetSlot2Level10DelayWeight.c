// bdc 0x08a2b8c4 BtlAiParamSetSlot2Level10DelayWeight
#include "bdc.h"

/* Level-10 entry of the `BtlAiParamSetSlot2` delay weight table (vtable `0x08af6c90` slot `+0x5c`):
   returns the weight of delay index `delayIndex` (0..5) for a CPU AI at skill level 10 — 100 for
   index 0 (a 0/30 s delay), 0 for every other index. */
s32 BtlAiParamSetSlot2Level10DelayWeight(BtlAiParamSet *self, s32 delayIndex)
{
    (void)self;
    if ((u32)(delayIndex - 1) < 5) {
        return 0;
    }
    return 100;
}
