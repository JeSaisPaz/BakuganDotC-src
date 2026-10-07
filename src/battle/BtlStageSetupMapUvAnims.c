// bdc 0x0889e3d8 BtlStageSetupMapUvAnims
#include "bdc.h"

/* Clears the four UV animation slots and sets up the scrolling backgrounds of the current arena
   g_btlArenaIndex with BtlStageSetupUvAnim: arenas 0..3 scroll the far views
   (`_f0_area00_farview_6/9/11`), 0xc `HDOP_bg2`, 0xd..0xf `f3_bg2_00`, 0x12/0x13 `bg1_00` plus the
   `f4_smoke01` smoke (type 4); other arenas get no UV animation. The scroll speed blocks
   {0, v, 0, 0} are written into the map models' rot / scale / velocity vectors.
   Called by BtlStageLoadMap. */

#define UV_SPEED_FARVIEW 0.00166666671f /* 0x3ada740e */
#define UV_SPEED_BG      0.000250000012f /* 0x3983126f */
#define UV_SPEED_SMOKE   0.00499999989f /* 0x3ba3d70a */

static void SetUvSpeed(float *params, float v)
{
    params[0] = 0.0f;
    params[1] = v;
    params[2] = 0.0f;
    params[3] = 0.0f;
}

void BtlStageSetupMapUvAnims(void)
{
    s32 arena = g_btlArenaIndex;
    s32 i;

    for (i = 0; i < 4; i++) {
        g_btlStageUvAnimParams[i] = NULL;
        g_btlStageUvAnimTypes[i] = 0;
    }

    switch (arena) {
    case 0:
        SetUvSpeed(g_btlArenaModels[0]->rot, UV_SPEED_FARVIEW);
        BtlStageSetupUvAnim(0, 2, "_f0_area00_farview_6", g_btlArenaModels[0]->rot, 0);
        SetUvSpeed(g_btlArenaModels[1]->rot, UV_SPEED_FARVIEW);
        BtlStageSetupUvAnim(1, 2, "_f0_area00_farview_6", g_btlArenaModels[1]->rot, 1);
        SetUvSpeed(g_btlArenaModels[1]->scale, UV_SPEED_BG);
        BtlStageSetupUvAnim(2, 2, "_f0_area00_farview_11", g_btlArenaModels[1]->scale, 1);
        BtlStageSetupUvAnim(3, 2, "_f0_area00_farview_9", g_btlArenaModels[1]->scale, 1);
        break;
    case 1:
    case 2:
    case 3:
        SetUvSpeed(g_btlArenaModels[0]->rot, UV_SPEED_FARVIEW);
        BtlStageSetupUvAnim(0, 2, "_f0_area00_farview_6", g_btlArenaModels[0]->rot, 0);
        SetUvSpeed(g_btlArenaModels[1]->rot, UV_SPEED_FARVIEW);
        BtlStageSetupUvAnim(1, 2, "_f0_area00_farview_6", g_btlArenaModels[1]->rot, 1);
        SetUvSpeed(g_btlArenaModels[1]->scale, UV_SPEED_BG);
        BtlStageSetupUvAnim(2, 2, "_f0_area00_farview_9", g_btlArenaModels[1]->scale, 1);
        break;
    case 0xc:
        SetUvSpeed(g_btlArenaModels[1]->scale, UV_SPEED_BG);
        BtlStageSetupUvAnim(2, 2, "HDOP_bg2", g_btlArenaModels[1]->scale, 1);
        break;
    case 0xd:
    case 0xe:
    case 0xf:
        SetUvSpeed(g_btlArenaModels[1]->scale, UV_SPEED_BG);
        BtlStageSetupUvAnim(0, 2, "f3_bg2_00", g_btlArenaModels[1]->scale, 1);
        break;
    case 0x12:
    case 0x13:
        SetUvSpeed(g_btlArenaModels[1]->scale, UV_SPEED_BG);
        BtlStageSetupUvAnim(2, 2, "bg1_00", g_btlArenaModels[1]->scale, 1);
        SetUvSpeed(g_btlArenaModels[1]->velocity, UV_SPEED_SMOKE);
        BtlStageSetupUvAnim(3, 4, "f4_smoke01", g_btlArenaModels[1]->velocity, 1);
        break;
    default:
        break;
    }
}
