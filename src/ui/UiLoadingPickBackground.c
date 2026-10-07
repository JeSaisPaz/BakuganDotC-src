// bdc 0x0890d360 UiLoadingPickBackground
#include "bdc.h"

/* Chooses the background/theme of the now-loading screen (task 10100 / 0x2774, `UiLoadingCtor`,
   update `UiLoadingUpdate`, draw `UiLoadingDraw`; shared objects `g_uiLoadingShared` from
   `UiLoadingInitShared`) from the story progress: scans `g_gameFieldJetStageTable` (pairs
   `{flag, stage}`, count `g_gameFieldJetStageCount`) backwards for the last flag set in
   `g_scriptGlobalBits` (`CoreBitsetTest`); stages 0/0xd select theme 0x12 and 0x12/10 select
   0x13 (`theme`, otherwise left as is), then lays out the background sprites. Sprites 0..22
   get flag 1 and have `maybe_billboardParams80` and `scaleX..angle` zeroed (bank C720 = 0); sprites
   1..5 and 17..20 get their position copied into `scaleX..angle` after the scale/rotation is set. */

void UiLoadingPickBackground(UiLoading *self)
{
  s32 stage;
  s32 i;
  s32 cell;
  float w;
  float h;
  GfxSprite *spr;
  GfxSprite *bg;
  GfxSprite *ball;

  stage = -1;
  for (i = g_gameFieldJetStageCount - 1; i >= 0; i--) {
    if (CoreBitsetTest(g_gameFieldJetStageTable[i][0], g_scriptGlobalBits)) {
      stage = (s32)g_gameFieldJetStageTable[i][1];
      break;
    }
  }
  if (stage == 0 || stage == 0xd) {
    self->theme = 0x12;
  } else if (stage == 0x12 || stage == 10) {
    self->theme = 0x13;
  }

  for (i = 0; i < 0x17; i++) {
    g_uiLoadingShared->sprites[i]->flags |= 1;
    spr = g_uiLoadingShared->sprites[i];
    spr->maybe_billboardParams80[0] = 0.0f;
    spr->maybe_billboardParams80[1] = 0.0f;
    spr->maybe_billboardParams80[2] = 0.0f;
    spr->maybe_billboardParams80[3] = 0.0f;
    spr = g_uiLoadingShared->sprites[i];
    spr->scaleX = 0.0f;
    spr->scaleY = 0.0f;
    spr->scaleZ = 0.0f;
    spr->angle = 0.0f;
  }
  g_uiLoadingShared->sprites[0]->flags &= ~1u;
  if (self->theme != 0x12) {
    g_uiLoadingShared->sprites[6]->flags &= ~1u;
    g_uiLoadingShared->sprites[7]->flags &= ~1u;
  }
  g_uiLoadingShared->sprites[6]->posZ += 2.0f;
  g_uiLoadingShared->sprites[7]->posZ += 2.0f;
  g_uiLoadingShared->sprites[10]->posZ -= 10.0f;
  g_uiLoadingShared->sprites[9]->posZ -= 10.0f;

  w = GfxSpriteGetWidth(g_uiLoadingShared->sprites[8]);
  bg = g_uiLoadingShared->sprites[8];
  h = GfxSpriteGetHeight(bg);
  UiSpriteSetSize(w * 2.0f, h * 2.0f, bg);
  GfxSpriteCenterPivot(g_uiLoadingShared->sprites[8]);
  g_uiLoadingShared->sprites[8]->flags |= 0x20;
  GfxSpriteInsetUv(0.5f, g_uiLoadingShared->sprites[8]);
  g_uiLoadingShared->sprites[8]->alpha = 0.5f;

  cell = -1;
  if (stage >= 0 && stage < 0x18) {
    cell = stage / 4;
  }
  if (cell < 0) {
    g_uiLoadingShared->sprites[10]->flags &= ~1u;
  } else {
    switch (cell) {
    case 0:
      GfxSpriteSetVCell(0.0f, g_uiLoadingShared->sprites[10]);
      break;
    case 1:
      GfxSpriteSetVCell(1.0f, g_uiLoadingShared->sprites[10]);
      break;
    case 2:
      GfxSpriteSetVCell(2.0f, g_uiLoadingShared->sprites[10]);
      break;
    case 3:
      GfxSpriteSetVCell(3.0f, g_uiLoadingShared->sprites[10]);
      break;
    case 4:
      GfxSpriteSetVCell(4.0f, g_uiLoadingShared->sprites[10]);
      break;
    case 5:
      GfxSpriteSetVCell(6.0f, g_uiLoadingShared->sprites[10]);
      break;
    }
  }

  ball = g_uiLoadingShared->sprites[1];
  GfxSpriteCenterPivot(ball);
  ball->flags |= 0x20;
  GfxSpriteInsetUv(0.5f, ball);
  ball->maybe_billboardParams80[0] = 1.0f;
  ball->maybe_billboardParams80[1] = 1.0f;
  ball->maybe_billboardParams80[2] = 0.0f;
  ball->maybe_billboardParams80[3] = 0.0f;
  GfxSpriteSetScaleRotation(ball, ball->maybe_billboardParams80[0],
                            ball->maybe_billboardParams80[1],
                            ball->maybe_billboardParams80[2], false);
  ball->scaleX = ball->posX;
  ball->scaleY = ball->posY;
  ball->scaleZ = ball->posZ;
  ball->angle = ball->posW;

  for (i = 0; i < 4; i++) {
    spr = g_uiLoadingShared->sprites[2 + i];
    GfxSpriteCenterPivot(spr);
    spr->flags |= 0x20;
    GfxSpriteInsetUv(0.5f, spr);
    if (i == 0 || i == 2) {
      spr->maybe_billboardParams80[0] = 1.0f;
      spr->maybe_billboardParams80[1] = 1.0f;
      spr->maybe_billboardParams80[2] = 0.5235988f;
      spr->maybe_billboardParams80[3] = 0.0f;
      spr->posX -= 16.0f;
    } else {
      spr->maybe_billboardParams80[0] = 1.0f;
      spr->maybe_billboardParams80[1] = 1.0f;
      spr->maybe_billboardParams80[2] = -0.5235988f;
      spr->maybe_billboardParams80[3] = 0.0f;
      spr->posX += 16.0f;
    }
    spr->posY -= 8.0f;
    GfxSpriteSetScaleRotation(spr, spr->maybe_billboardParams80[0],
                              spr->maybe_billboardParams80[1],
                              spr->maybe_billboardParams80[2], false);
    spr->scaleX = spr->posX;
    spr->scaleY = spr->posY;
    spr->scaleZ = spr->posZ;
    spr->angle = spr->posW;
  }

  for (i = 0; i < 4; i++) {
    spr = g_uiLoadingShared->sprites[17 + i];
    GfxSpriteCenterPivot(spr);
    spr->flags |= 0x20;
    GfxSpriteInsetUv(0.5f, spr);
    spr->maybe_billboardParams80[0] = 1.0f;
    spr->maybe_billboardParams80[1] = 1.0f;
    spr->maybe_billboardParams80[2] = 0.0f;
    spr->maybe_billboardParams80[3] = 0.0f;
    /* scale/rotation taken from sprite 1, not from spr itself */
    GfxSpriteSetScaleRotation(spr, ball->maybe_billboardParams80[0],
                              ball->maybe_billboardParams80[1],
                              ball->maybe_billboardParams80[2], false);
    spr->posZ += 10.0f;
    spr->scaleX = spr->posX;
    spr->scaleY = spr->posY;
    spr->scaleZ = spr->posZ;
    spr->angle = spr->posW;
  }
}
