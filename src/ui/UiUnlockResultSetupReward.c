// bdc 0x089389c4 UiUnlockResultSetupReward
#include "bdc.h"

/* Builds the reward display of the unlock-result screen (task 375, `UiUnlockResultCtor`):
   clears the tween area `tweens`, creates the layout-0x11 sprites, the `"part_comp_moji"` caption
   sprite (sprite 0x23, 256x64) and a heap copy of it (sprite 0x24), hides all 0x25 sprites, then
   by `rewardKind` centres the frame sprites 2..4 and records `frameEdges`; kinds other than 5/6
   also lay out the corner sprites 6..11 around sprite 12 (`cornerOffset`). Kind 5 loads a Maxus
   part model (`g_uiMaxusPartModelNames`, set `g_rewardMaxusSet`), kind 6 a figure model
   (`g_uiUnlockFigureModelNames`, `rewardIndex % 20`), both through `GfxModelCtor` into
   `model`, with rotation, specular and material callback set up. An out-of-range `rewardIndex`
   or a model missing from the loaded packs sets phase 3 and reports result 0
   (`UiUnlockResultSetResult`) and returns. */

#define SPRITES ((GfxSprite **)self->base.data)

void UiUnlockResultSetupReward(UiUnlockResult *self)
{
  GfxSpriteLayer *layer;
  void *texture;
  bool fromLow;
  GfxSprite *sprite;
  GfxModel *model;
  u32 i;
  u8 kind;
  float pos[4] __attribute__((aligned(16)));
  float uv[4];
  float specular[4] __attribute__((aligned(16)));
  char name[0x40];
  const char *partNames[12];
  const char *figureNames[45];

  memset(self->tweens, 0, sizeof(self->tweens));
  UiLayoutCreateSprites(self->base.spriteLayer, self->base.data, 0x11);
  layer = self->base.spriteLayer;
  texture = GfxFindTexture("part_comp_moji");
  pos[0] = 112.0f;
  pos[1] = 112.0f;
  pos[2] = SPRITES[6]->posZ;
  pos[3] = 0.0f;
  SPRITES[0x23] = GfxSpriteLayerCreateSprite(layer, texture, pos, false);
  uv[0] = 0.0f;
  uv[1] = 0.0f;
  uv[2] = 256.0f;
  uv[3] = 40.0f;
  GfxSpriteSetUvRectXYWH(SPRITES[0x23], uv);
  UiSpriteSetSize(256.0f, 64.0f, SPRITES[0x23]);
  GfxSpriteCenterPivot(SPRITES[0x23]);
  GfxSpriteResetMatrix(SPRITES[0x23]);

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  sprite = MemAlloc(sizeof(GfxSprite), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (sprite != NULL)
    GfxSpriteCtor(sprite);
  SPRITES[0x24] = sprite;
  GfxSpriteLayerAdd(self->base.spriteLayer, (CoreObject *)SPRITES[0x24]);
  GfxSpriteCopy(SPRITES[0x23], SPRITES[0x24]);

  for (i = 0; i < 0x25; i++) {
    SPRITES[i]->flags &= ~1u;
    SPRITES[i]->alpha = 0.0f;
  }

  kind = self->rewardKind;
  switch (kind) {
  case 0: case 1: case 2: case 3: case 4: case 7: case 8: case 9:
    for (i = 2; i < 5; i++) {
      GfxSpriteCenterPivot(SPRITES[i]);
      UiSpriteSetScaleRotation(SPRITES[i], 1.0f, 1.0f, 0.0f);
    }
    self->frameEdges[0] = SPRITES[4]->posX;
    self->frameEdges[1] = UiAbsDiff(SPRITES[3]->posY, SPRITES[2]->posY);
    self->frameEdges[2] = SPRITES[2]->posX;
    self->frameEdges[3] = UiAbsDiff(SPRITES[3]->posY, SPRITES[4]->posY);
    GfxSpriteCenterPivot(SPRITES[12]);
    UiSpriteSetScaleRotation(SPRITES[12], 1.0f, 1.0f, 0.0f);
    for (i = 6; i < 12; i++) {
      GfxSpriteCenterPivot(SPRITES[i]);
      UiSpriteSetScaleRotation(SPRITES[i], 1.0f, 1.0f, 0.0f);
      self->cornerOffset[i - 6][0] = UiAbsDiff(SPRITES[12]->posX, SPRITES[i]->posX);
      self->cornerOffset[i - 6][1] = UiAbsDiff(SPRITES[12]->posY, SPRITES[i]->posY);
    }
    kind = self->rewardKind;
    break;

  case 5:
    name[0] = 0;
    memset(name + 1, 0, sizeof(name) - 1);
    for (i = 2; i < 5; i++) {
      GfxSpriteCenterPivot(SPRITES[i]);
      UiSpriteSetScaleRotation(SPRITES[i], 1.0f, 1.0f, 0.0f);
    }
    self->frameEdges[0] = SPRITES[4]->posX;
    self->frameEdges[1] = UiAbsDiff(SPRITES[3]->posY, SPRITES[2]->posY);
    self->frameEdges[2] = SPRITES[2]->posX;
    self->frameEdges[3] = UiAbsDiff(SPRITES[3]->posY, SPRITES[4]->posY);
    if (self->rewardIndex >= 12) {
      self->base.phase = 3;
      UiUnlockResultSetResult(self);
      return;
    }
    /* Only the half of the table for the current set is filled. */
    if (g_rewardMaxusSet == 0) {
      partNames[0] = g_uiMaxusPartModelNames[2];
      partNames[1] = g_uiMaxusPartModelNames[3];
      partNames[2] = g_uiMaxusPartModelNames[1];
      partNames[3] = g_uiMaxusPartModelNames[4];
      partNames[4] = g_uiMaxusPartModelNames[0];
      partNames[5] = g_uiMaxusPartModelNames[5];
      strncpy(name, partNames[self->rewardIndex], 0x40);
      if (CorePackChainFind(g_ioLzsPackages, name) == NULL) {
        self->rewardIndex = 0;
        strncpy(name, partNames[0], 0x40);
      }
    } else if (g_rewardMaxusSet == 1) {
      partNames[6] = g_uiMaxusPartModelNames[6];
      partNames[7] = g_uiMaxusPartModelNames[7];
      partNames[8] = g_uiMaxusPartModelNames[8];
      partNames[9] = g_uiMaxusPartModelNames[9];
      partNames[10] = g_uiMaxusPartModelNames[10];
      partNames[11] = g_uiMaxusPartModelNames[11];
      strncpy(name, partNames[self->rewardIndex], 0x40);
      if (CorePackChainFind(g_ioLzsPackages, name) == NULL) {
        self->rewardIndex = 0;
        strncpy(name, partNames[6], 0x40);
      }
    }
    if (CorePackChainFind(g_ioLzsPackages, name) == NULL) {
      self->base.phase = 3;
      UiUnlockResultSetResult(self);
      return;
    }
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    model = MemAlloc(sizeof(GfxModel), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (model != NULL)
      GfxModelCtor(model, name, 0);
    self->model = model;
    kind = self->rewardKind;
    break;

  case 6:
    name[0] = 0;
    memset(name + 1, 0, sizeof(name) - 1);
    for (i = 2; i < 5; i++) {
      GfxSpriteCenterPivot(SPRITES[i]);
      UiSpriteSetScaleRotation(SPRITES[i], 1.0f, 1.0f, 0.0f);
    }
    self->frameEdges[0] = SPRITES[4]->posX;
    self->frameEdges[1] = UiAbsDiff(SPRITES[3]->posY, SPRITES[2]->posY);
    self->frameEdges[2] = SPRITES[2]->posX;
    self->frameEdges[3] = UiAbsDiff(SPRITES[3]->posY, SPRITES[4]->posY);
    if (self->rewardIndex >= 44) {
      self->base.phase = 3;
      UiUnlockResultSetResult(self);
      return;
    }
    memcpy(figureNames, g_uiUnlockFigureModelNames, sizeof(figureNames));
    self->rewardIndex = self->rewardIndex % 20;
    strncpy(name, figureNames[self->rewardIndex], 0x40);
    if (CorePackChainFind(g_ioLzsPackages, (char *)figureNames[self->rewardIndex]) == NULL) {
      self->rewardIndex = 20;
      strncpy(name, figureNames[20], 0x40);
    }
    if (CorePackChainFind(g_ioLzsPackages, name) == NULL) {
      self->base.phase = 3;
      UiUnlockResultSetResult(self);
      return;
    }
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    model = MemAlloc(sizeof(GfxModel), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (model != NULL)
      GfxModelCtor(model, figureNames[self->rewardIndex], 0);
    self->model = model;
    kind = self->rewardKind;
    break;
  }

  if (kind != 5 && kind != 6)
    return;

  ((GfxModel *)self->model)->lighting = 1;
  if (self->rewardKind == 6) {
    model = self->model;
    if (self->rewardIndex == 0 || self->rewardIndex == 12 || self->rewardIndex == 20) {
      model->rot[0] = 1.5707964f;
      model->rot[1] = 2.042035f;
      model->rot[2] = 1.5707964f;
      model->rot[3] = 0.0f;
    } else {
      model->rot[0] = 1.5707964f;
      model->rot[1] = 1.5707964f;
      model->rot[2] = 1.5707964f;
      model->rot[3] = 0.0f;
    }
    specular[0] = 0.1f;
    specular[1] = 0.1f;
    specular[2] = 0.1f;
    specular[3] = 1.0f;
    GfxModelSetSpecular(8.0f, self->model, specular, NULL);
    model = self->model;
    model->ambient[3] = 1.0f;
    model->ambient[0] = 0.45f;
    model->ambient[1] = 0.45f;
    model->ambient[2] = 0.45f;
    GfxModelForEachMaterial(self->model, (void *)UiUnlockResultKind6MaterialCallback, NULL);
  } else if (self->rewardKind == 5) {
    model = self->model;
    if (g_rewardMaxusSet == 0) {
      model->rot[0] = 1.2566371f;
      model->rot[1] = 2.1991148f;
      model->rot[2] = 1.5707964f;
      model->rot[3] = 0.0f;
    } else if (g_rewardMaxusSet == 1) {
      model->rot[0] = 1.2566371f;
      model->rot[1] = 2.1991148f;
      model->rot[2] = 1.5707964f;
      model->rot[3] = 0.0f;
    }
    specular[0] = 0.6f;
    specular[1] = 0.6f;
    specular[2] = 0.6f;
    specular[3] = 1.0f;
    GfxModelSetSpecular(10.0f, self->model, specular, NULL);
    GfxModelForEachMaterial(self->model, (void *)UiUnlockResultModelMaterialCallback, NULL);
  }
  model = self->model;
  self->modelBaseY = model->rot[1];
  model->ambient[3] = 0.0f;
}
