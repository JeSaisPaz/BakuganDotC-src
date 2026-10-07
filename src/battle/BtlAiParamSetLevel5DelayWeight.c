// bdc 0x08a2a7bc BtlAiParamSetLevel5DelayWeight
#include "bdc.h"

/* Level-5 entry of the `BtlAiParamSet` delay weight table (vtable `0x08af62c8` slot `+0x34`):
   returns the weight of delay index `delayIndex` (0..5) for a CPU AI at skill level 5 — 100 for
   index 2 (a 2/30 s delay), 0 for every other index. */
s32 BtlAiParamSetLevel5DelayWeight(BtlAiParamSet *self, s32 delayIndex)
{
    (void)self;
    switch (delayIndex) {
    case 2:
        return 100;
    default:
        return 0;
    }
}
