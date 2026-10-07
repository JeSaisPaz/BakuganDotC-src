// bdc 0x088d4778 GameStageBuild
#include "bdc.h"

/* Builds stage `stage` for the field (caller `GameFieldPhaseLoad`, with the field's model list):
   sets the current stage `g_gameStageIndex` (`GameStageFindEntry`, result unused), clears
   `g_gameStageExtraModels` [0..1] and `g_gameStageExtraModelFlag`, points `g_gfxFogParams` at
   `g_gameStageFogParams``[stage]` and expands its packed colour into `g_gameStageClearColor` and the
   display clear colour; on the hub stage 0x20 picks the cabin background `menu_cabin_bg_f*_*.gmo` by
   the latest set flag of `g_gameFieldJetStageTable` (extra model 1); creates the main
   `adventure_map.gmo` prop (extra model 0; `GameStageCreateProp`, flags28 bit 0x100,
   `GameStageWallMaterialCallback` on `f3_quest_wall_re05_comp`, `locator1` node position * 0.1 into
   `g_gameStageLocatorPos`) and, if present in the packages, `adventure_subset_1.gmo` (extra model 1);
   copies the stage lights (`GameGetStageLight`) into the scene light globals, zeroes (bank C720) `g_gameStageLocatorPos`, loads `adventure_map.ctc` collision
   (`GameStageCreateCollider`), sets `g_stageObjList`, spawns layout objects, binds toon textures,
   adds sky billboards outside stages 20..27/36..39, sets `g_btlArenaIndex`, creates the lens-flare
   task 482 (with the field's sprite layer) outside stages 0/13 and 4..7, and applies the extra-model
   flag (`GameStageSetExtraModelFlag`). */

void GameStageBuild(s32 stage, void *list)
{
  s32 i;
  s32 jet;
  s32 cabin;
  u32 color;
  GfxDisplay *display;
  GfxModel *model;
  GmoNode *node;
  CoreObject *obj;
  BtlStageLight *light;
  CoreTask *task;
  GameFieldTask *field;

  g_gameStageIndex = stage;
  GameStageFindEntry(stage);
  for (i = 0; i < 2; i++) {
    g_gameStageExtraModels[i] = NULL;
  }
  g_gameStageExtraModelFlag = 0;
  g_gfxFogParams = &g_gameStageFogParams[stage];
  color = g_gfxFogParams->color;
  for (i = 0; i < 4; i++) {
    g_gameStageClearColor[i] =
        (float)(s32)(((color >> (i * 8)) & 0xffu) * 0x01010101u >> 1) / 2147483648.0f;
  }
  display = g_gfxDisplay;
  for (i = 0; i < 4; i++) {
    display->clearColor[i] = g_gameStageClearColor[i];
  }

  if (g_gameStageIndex == 0x20) {
    jet = -1;
    for (i = g_gameFieldJetStageCount - 1; i >= 0; i--) {
      if (CoreBitsetTest(g_gameFieldJetStageTable[i][0], g_scriptGlobalBits)) {
        jet = g_gameFieldJetStageTable[i][1];
        break;
      }
    }
    switch (jet) {
    case 0: cabin = 0; break;
    case 1: cabin = 1; break;
    case 2: cabin = 1; break;
    case 4: cabin = 2; break;
    case 5: cabin = 2; break;
    case 6: cabin = 2; break;
    case 8: cabin = 3; break;
    case 9: cabin = 3; break;
    case 10: cabin = 4; break;
    case 12: cabin = 5; break;
    case 13: cabin = 6; break;
    case 14: cabin = 5; break;
    case 16: cabin = 7; break;
    case 17: cabin = 7; break;
    case 18: cabin = 8; break;
    case 20: cabin = 4; break;
    case 24: cabin = 4; break;
    default: cabin = -1; break;
    }
    if (cabin >= 0) {
      obj = GameStageCreateProp(g_gameStageModelNames[cabin + 2],
                                g_gameStagePropPositions[g_gameStageIndex], list, true);
      g_gameStageExtraModels[1] = (u8 *)obj;
      ((GfxModel *)obj)->lighting = 0;
    }
  }

  model = (GfxModel *)GameStageCreateProp(g_gameStageModelNames[0],
                                          g_gameStagePropPositions[g_gameStageIndex], list, true);
  GmoModelSetFlags28(model->data, 0x100, 0xffffffff);
  g_gameStageExtraModels[0] = (u8 *)model;
  GfxModelForEachMaterialByName(model, "f3_quest_wall_re05_comp", GameStageWallMaterialCallback, NULL);
  node = (GmoNode *)GfxModelFindNode(model, "locator1");
  if (node != NULL) {
    g_gameStageLocatorPos[0] = ((float *)node->block38)[0] * 0.1f;
    g_gameStageLocatorPos[1] = ((float *)node->block38)[1] * 0.1f;
    g_gameStageLocatorPos[2] = ((float *)node->block38)[2] * 0.1f;
  }
  ActorApplyStageLight(model);

  if (CorePackChainFindData(g_ioLzsPackages, g_gameStageModelNames[1]) != NULL) {
    obj = GameStageCreateProp(g_gameStageModelNames[1], (float *)&g_gfxVecZero, list, true);
    ActorApplyStageLight(obj);
    g_gameStageExtraModels[1] = (u8 *)obj;
  }

  light = (BtlStageLight *)GameGetStageLight(-1);
  g_gfxLightDir0[0] = -light->dir[0];
  g_gfxLightDir0[1] = -light->dir[1];
  g_gfxLightDir0[2] = -light->dir[2];
  g_gfxLightDir0[3] = 0.0f;
  light = (BtlStageLight *)GameGetStageLight(-1);
  g_gfxLightDir1[0] = -light->dir2[0];
  g_gfxLightDir1[1] = -light->dir2[1];
  g_gfxLightDir1[2] = -light->dir2[2];
  g_gfxLightDir1[3] = 0.0f;
  light = (BtlStageLight *)GameGetStageLight(-1);
  g_gfxLightColor0[0] = light->color[0];
  g_gfxLightColor0[1] = light->color[1];
  g_gfxLightColor0[2] = light->color[2];
  g_gfxLightColor0[3] = 1.0f;
  light = (BtlStageLight *)GameGetStageLight(-1);
  g_gfxLightColor1[0] = light->color2[0];
  g_gfxLightColor1[1] = light->color2[1];
  g_gfxLightColor1[2] = light->color2[2];
  g_gfxLightColor1[3] = 1.0f;
  light = (BtlStageLight *)GameGetStageLight(-1);
  g_gfxAmbientColor[0] = light->ambient[0];
  g_gfxAmbientColor[1] = light->ambient[1];
  g_gfxAmbientColor[2] = light->ambient[2];
  g_gfxAmbientColor[3] = 1.0f;
  /* bank C720 = (0, 0, 0, 0) overwrites the locator position */
  g_gameStageLocatorPos[0] = 0.0f;
  g_gameStageLocatorPos[1] = 0.0f;
  g_gameStageLocatorPos[2] = 0.0f;
  g_gameStageLocatorPos[3] = 0.0f;

  GameStageCreateCollider("adventure_map.ctc");
  g_stageObjList = (CoreObjectList *)list;
  GameStageSpawnLayoutObjects();
  GameStageBindToonTextures();
  if (GameStageIs20To27Or36To39() == 0) {
    GameStageCreateSkyBillboards(g_gameStageIndex);
  }
  g_btlArenaIndex = stage;
  if (GameStageIs0Or13() == 0 && GameStageIs4To7() == 0) {
    task = CoreTaskCreate(0x1e2, 100);
    field = (GameFieldTask *)GameFieldFindTask();
    ((GfxLensFlareTask *)task)->layer = field->spriteLayer;
  }
  GameStageSetExtraModelFlag(g_gameStageExtraModelFlag);
}
