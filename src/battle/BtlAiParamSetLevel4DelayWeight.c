// bdc 0x08a2a770 BtlAiParamSetLevel4DelayWeight
#include "bdc.h"

/* Level-4 entry of the `BtlAiParamSet` delay weight table: returns the weight of delay index
   `delayIndex` (0..5) for a CPU AI at skill level 4 -- 100 for index 2 (a 2/30 s delay), 0 for
   every other index. */
s32 BtlAiParamSetLevel4DelayWeight(BtlAiParamSet *self, s32 delayIndex)
{
    (void)self;
    if (delayIndex == 2) {
        return 100;
    }
    return 0;
}
