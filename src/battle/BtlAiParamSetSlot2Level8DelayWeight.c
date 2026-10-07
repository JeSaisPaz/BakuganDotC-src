// bdc 0x08a2b834 BtlAiParamSetSlot2Level8DelayWeight
#include "bdc.h"

/* Level-8 entry of the `BtlAiParamSetSlot2` delay weight table (vtable `0x08af6c90` slot `+0x4c`):
   returns the weight of delay index `delayIndex` (0..5) for a CPU AI at skill level 8 — 100 for
   index 0 (a 0/30 s delay), 0 for every other index. */
s32 BtlAiParamSetSlot2Level8DelayWeight(BtlAiParamSet *self, s32 delayIndex)
{
    (void)self;
    if ((u32)(delayIndex - 1) < 5) {
        return 0;
    }
    return 100;
}
