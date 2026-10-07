// bdc 0x0889dc88 BtlStageGetArenaBounds
#include "bdc.h"

/* Returns the bounding box `{min x, y, z; max x, y, z}` of the current arena
   (`g_btlArenaBounds` indexed by `g_btlArenaIndex`). */
float *BtlStageGetArenaBounds(void)
{
    return g_btlArenaBounds[g_btlArenaIndex];
}
