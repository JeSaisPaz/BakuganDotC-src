// bdc 0x088ff3b4 BtlGetStageAmbientColor
#include "bdc.h"

/* Writes the colour vector of the current battle id (script global variable 1, 0..39) from
   `g_btlStageAmbientColors` into `out`, or zeroes for other ids. */
void BtlGetStageAmbientColor(float *out)
{
    s32 battleId;
    const float *src;

    out[2] = 0.0f;
    out[1] = 0.0f;
    out[0] = 0.0f;
    out[3] = 0.0f;
    battleId = g_scriptGlobalVars[1];
    if (battleId >= 0 && battleId < 40) {
        src = g_btlStageAmbientColors[battleId];
        out[0] = src[0];
        out[1] = src[1];
        out[2] = src[2];
        out[3] = src[3];
    }
}
