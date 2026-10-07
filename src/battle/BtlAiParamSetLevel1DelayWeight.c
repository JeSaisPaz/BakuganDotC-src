// bdc 0x08a2a6a4 BtlAiParamSetLevel1DelayWeight
#include "bdc.h"

/* Level-1 method (entry 2) of the AI delay-weight parameter set (vtable `0x08af62c8`, see
   `BtlAiApplyLevel`): returns weight 100 for `delayIndex` 5 and 0 for every other index. */
s32 BtlAiParamSetLevel1DelayWeight(BtlAiParamSet *self, s32 delayIndex)
{
    (void)self;
    if (delayIndex == 5) {
        return 100;
    }
    return 0;
}
