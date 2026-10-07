// bdc 0x0889d4a4 BtlStageGetLightInfo
#include "bdc.h"

/* Returns the lighting record of arena `arena` (the current arena `g_btlArenaIndex` when -1)
   from `g_btlStageLights`: e.g. direction (0.53, -0.70, 0.45), light colour (0.3, 0.3, 0.6),
   a second direction, a second colour and the ambient colour (0.06/0.07/0.15). */
BtlStageLight *BtlStageGetLightInfo(s32 arena)
{
    if (arena == -1) {
        arena = g_btlArenaIndex;
    }
    return &g_btlStageLights[arena];
}
