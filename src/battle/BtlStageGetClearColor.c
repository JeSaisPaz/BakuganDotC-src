// bdc 0x0889e908 BtlStageGetClearColor
#include "bdc.h"

/* Converts the current arena's RGB bytes (`g_btlArenaClearColorRgb[g_btlArenaIndex]`) to floats
   scaled by 1/255 into `g_btlStageClearColor` (alpha 0) and returns that colour;
   `BtlStageLoadMap` copies it into the display clear colour. */
float *BtlStageGetClearColor(void)
{
    const u8 *rgb = g_btlArenaClearColorRgb[g_btlArenaIndex];

    g_btlStageClearColor[3] = 0.0f;
    g_btlStageClearColor[0] = (float)rgb[0] * 0.00392156979f;
    g_btlStageClearColor[1] = (float)rgb[1] * 0.00392156979f;
    g_btlStageClearColor[2] = (float)rgb[2] * 0.00392156979f;
    return g_btlStageClearColor;
}
