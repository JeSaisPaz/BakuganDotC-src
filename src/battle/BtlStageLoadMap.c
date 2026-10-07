// bdc 0x0889e980 BtlStageLoadMap
#include "bdc.h"

/* Builds battle arena `arena` (stored in `g_btlArenaIndex`) with its models appended to `list`:
   clears `g_btlArenaModels` and the map visibility, selects the arena fog record (unpacked to the
   float `g_gfxFogColor` in the VFPU), copies the arena clear colour into the display, loads
   `battle_map.gmo` and, when present in the pack chain, `battle_subset_1.gmo` / `battle_subset_2.gmo`,
   sets the effect tint (`0x80c0a0a0` on stages 0/13, else untouched), fills the scene light vectors and
   colours from the arena light record, creates the `battle_map.ctc` collider, spawns the placed objects,
   binds the toon textures and UV animations, clears profile words 10 and 0x1c, starts the battle event
   script `battle/<folder>/event_battle_<id>.script` in rule mode 1 (arenas below 0x29; profile word 10 =
   the event's hundreds digit when 1 or 2), builds the stop walls, the sky sprites (not on stages
   20..27/36..39) and the lens-flare task 0x1e2 (not on stages 4..7, 20..27/36..39, 0/13), and
   reapplies the map visibility. */

void BtlStageLoadMap(s32 arena, CoreObjectList *list)
{
    char path[84];
    GfxDisplay *display;
    float *clearColor;
    CoreObject *model;
    BtlStageLight *light;
    GfxLensFlareTask *flare;
    BtlMain *mainTask;
    s32 eventId;
    s32 digit;
    s32 i;
    u32 packed;

    g_btlArenaIndex = arena;
    BtlStageFindArenaEntry(arena);
    for (i = 0; i < 3; i++) {
        g_btlArenaModels[i] = NULL;
    }
    g_btlMapVisible = 0;
    g_gfxFogParams = &g_btlArenaFogParams[arena];
    /* packed RGBA bytes -> float vec4 (vuc2i: byte * 0x01010101 >> 1, then vi2f by 2^31) */
    packed = g_gfxFogParams->color;
    for (i = 0; i < 4; i++) {
        u32 b = (packed >> (i * 8)) & 0xff;
        g_gfxFogColor[i] = (float)(int)(b * 0x01010101u >> 1) / 2147483648.0f;
    }
    display = g_gfxDisplay;
    clearColor = BtlStageGetClearColor();
    for (i = 0; i < 4; i++) {
        display->clearColor[i] = clearColor[i];
    }

    model = BtlStageCreateMapModel(g_btlMapModelNames[0], &g_gfxVecZero.x, list, 1);
    GmoModelSetFlags28(((GfxModel *)model)->data, 0x100, 0xffffffff);
    g_btlArenaModels[0] = (GfxModel *)model;
    BtlStageSetupModelMaterials(model);
    BtlStageApplyLightColors(model);
    if (GameStageIs0Or13() != 0) {
        GfxSetEffectTint(0x80c0a0a0);
    } else {
        GfxSetEffectTint(0xffffffff);
    }

    light = BtlStageGetLightInfo(-1);
    g_gfxLightDir0[0] = -light->dir[0];
    g_gfxLightDir0[1] = -light->dir[1];
    g_gfxLightDir0[2] = -light->dir[2];
    g_gfxLightDir0[3] = 0.0f;
    light = BtlStageGetLightInfo(-1);
    g_gfxLightDir1[0] = -light->dir2[0];
    g_gfxLightDir1[1] = -light->dir2[1];
    g_gfxLightDir1[2] = -light->dir2[2];
    g_gfxLightDir1[3] = 0.0f;
    light = BtlStageGetLightInfo(-1);
    g_gfxLightColor0[0] = light->color[0];
    g_gfxLightColor0[1] = light->color[1];
    g_gfxLightColor0[2] = light->color[2];
    g_gfxLightColor0[3] = 1.0f;
    light = BtlStageGetLightInfo(-1);
    g_gfxLightColor1[0] = light->color2[0];
    g_gfxLightColor1[1] = light->color2[1];
    g_gfxLightColor1[2] = light->color2[2];
    g_gfxLightColor1[3] = 1.0f;
    light = BtlStageGetLightInfo(-1);
    g_gfxAmbientColor[0] = light->ambient[0];
    g_gfxAmbientColor[1] = light->ambient[1];
    g_gfxAmbientColor[2] = light->ambient[2];
    g_gfxAmbientColor[3] = 1.0f;

    if (CorePackChainFindData(g_ioLzsPackages, g_btlMapModelNames[1]) != NULL) {
        model = BtlStageCreateMapModel(g_btlMapModelNames[1], &g_gfxVecZero.x, list, 1);
        g_btlArenaModels[1] = (GfxModel *)model;
        BtlStageSetupModelMaterials(model);
        BtlStageApplyLightColors(model);
    }
    if (CorePackChainFindData(g_ioLzsPackages, g_btlMapModelNames[2]) != NULL) {
        model = BtlStageCreateMapModel(g_btlMapModelNames[2], &g_gfxVecZero.x, list, 1);
        g_btlArenaModels[2] = (GfxModel *)model;
        BtlStageSetupModelMaterials(model);
        BtlStageApplyLightColors(model);
    }

    for (i = 0; i < 4; i++) {
        g_btlArenaEffectExtentsCur[i] = g_btlArenaEffectExtents[g_btlArenaIndex][i];
    }
    BtlStageCreateMapCollider("battle_map.ctc");
    g_stageObjList = list;
    BtlStageSpawnPlacedObjects();
    BtlStageBindToonTextures();
    BtlStageSetupMapUvAnims();
    g_btlMapFlashState = 0;
    SaveProfileSetWord(SaveGetProfile(), 10, 0);
    SaveProfileSetWord(SaveGetProfile(), 0x1c, 0);

    if (arena < 0x29 && g_scriptGlobalVars[8] == 1) {
        eventId = g_scriptGlobalVars[9];
        sprintf(path, "battle/%s/event_battle_%06d.script", g_btlFieldFolderNames[eventId / 100000],
                eventId);
        g_btlEventScript = ScriptSpawn(path);
        digit = (eventId / 100) % 10;
        if (digit < 2) {
            if (digit > 0) {
                SaveProfileSetWord(SaveGetProfile(), 10, 1);
            }
        } else if (digit < 3) {
            SaveProfileSetWord(SaveGetProfile(), 10, 2);
        }
    }

    BtlStageCreateStopWalls();
    g_btlSkySprites[1] = NULL;
    g_btlSkySprites[0] = NULL;
    if (GameStageIs20To27Or36To39() == 0) {
        BtlStageCreateSkyLights(g_btlArenaIndex);
    }
    if (GameStageIs4To7() == 0 && GameStageIs20To27Or36To39() == 0 && GameStageIs0Or13() == 0) {
        flare = (GfxLensFlareTask *)CoreTaskCreate(0x1e2, 100);
        mainTask = BtlGetCameraTask();
        flare->layer = mainTask->spriteLayers[0];
    }
    BtlStageSetMapVisible(g_btlMapVisible);
}
