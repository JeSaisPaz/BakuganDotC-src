// bdc 0x0889e8c8 BtlStageSetMapVisible
#include "bdc.h"

/* Stores `visible` in `g_btlMapVisible` and in byte `+0xbc` of each loaded arena model of
   `g_btlArenaModels` (the field `GfxModel` names `lighting`; here used as the map's visible
   flag). */
void BtlStageSetMapVisible(u8 visible)
{
    int i;

    g_btlMapVisible = visible;
    for (i = 0; i < 3; i++) {
        if (g_btlArenaModels[i] != NULL) {
            g_btlArenaModels[i]->lighting = visible;
        }
    }
}
