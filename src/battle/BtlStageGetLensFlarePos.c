// bdc 0x0889e814 BtlStageGetLensFlarePos
#include "bdc.h"

/* Writes the sun position of the current arena (primaryPos of its `g_btlStageSkyTable`
   entry) to `out` and returns 1; on arena 8 it applies the same adjustment as
   `BtlStageCreateSkyLights` (xyz scaled by 0.9, then y by 0.6, and out[3] set to 0).
   Returns 0 without writing on stages 0 and 13 (`GameStageIs0Or13`).
   Called by `GfxLensFlareTaskCtor`. */

int BtlStageGetLensFlarePos(float *out)
{
    const BtlStageSkyEntry *entry;

    if (GameStageIs0Or13() != 0) {
        return 0;
    }
    entry = &g_btlStageSkyTable[g_btlArenaIndex];
    out[0] = entry->primaryPos[0];
    out[1] = entry->primaryPos[1];
    out[2] = entry->primaryPos[2];
    if (g_btlArenaIndex == 8) {
        /* vscl.t into C710: the w lane stored is the bank's S713 = 0 */
        out[0] = out[0] * 0.899999976f; /* 0x3f666666 */
        out[1] = out[1] * 0.899999976f;
        out[2] = out[2] * 0.899999976f;
        out[3] = 0.0f;
        out[1] = out[1] * 0.600000024f; /* 0x3f19999a */
    }
    return 1;
}
