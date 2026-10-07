// bdc 0x0889d480 BtlStageGetFogParams
#include "bdc.h"

/* Returns the current arena's fog record (`g_btlArenaFogParams` indexed by
   `g_btlArenaIndex`). Used by `BtlStageUpdateAmbientEffects`, `BtlFinishTaskUpdate` and
   `BtlDemoDrawPlay`. */
void *BtlStageGetFogParams(void)
{
    return &g_btlArenaFogParams[g_btlArenaIndex];
}
