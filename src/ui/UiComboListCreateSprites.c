// bdc 0x089b3688 UiComboListCreateSprites
#include "bdc.h"

/* Sets up the sprite table (`base.data`, 0x4d layout sprites plus 24 created ones) of the combo-list
   screen (task 3002, `UiComboList`; called from `UiComboListOpenPhase`): clears flag bit 0 of
   sprites 0..0x4c, sets it on sprites 0 and 3, then for each of the text sprites 1, 2 and 4 creates
   eight 0x160-byte `GfxSprite`s (slots 0x4d.., 0x55.., 0x5d..) on the screen layer, copies of the
   source sprite (`GfxSpriteCopy`) tinted `g_colorBlack`, offset by 2 pixels in the eight
   directions (a black outline drawn behind: the source's posZ drops by 1 per copy). Sprite 4 first gets
   cell `combo - 1` (`GfxSpriteSetVCell`) and y 215. Then ORs 0x20 into every layer sprite's flags,
   fills the combo slots (`UiComboListUpdateSlots`), sets bit 0 on sprites 5..10, 13..24 and,
   for combos 1, 3, 4, 6, 14, 15, 16 and 18, 25..30 (other combos shift sprites 0x35 and 0x39 right by
   1.5 widths - 2), and finishes sprites 0x45, 0x47..0x4b (cell 1, alpha 0.5, y 272, centred pivot). */

void UiComboListCreateSprites(UiComboList *self)

{
  GfxSprite *spr;
  GfxSprite *copy;
  bool wasLow;
  float x;
  float width;
  int i;

  for (i = 0; i < 0x4d; i++) {
    ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
  }
  ((GfxSprite **)self->base.data)[0]->flags |= 1;
  ((GfxSprite **)self->base.data)[3]->flags |= 1;

  for (i = 0; i < 8; i++) {
    ((GfxSprite **)self->base.data)[1]->flags |= 1;
    MemLock();
    wasLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    spr = MemAlloc(0x160, (char *)0x0, 0);
    MemSetAllocFromLow(wasLow);
    MemUnlock();
    copy = NULL;
    if (spr != NULL) {
      GfxSpriteCtor(spr);
      copy = spr;
    }
    ((GfxSprite **)self->base.data)[0x4d + i] = copy;
    GfxSpriteLayerAdd(self->base.spriteLayer,
                      (CoreObject *)((GfxSprite **)self->base.data)[0x4d + i]);
    GfxSpriteCopy(((GfxSprite **)self->base.data)[1],
                  ((GfxSprite **)self->base.data)[0x4d + i]);
    spr = ((GfxSprite **)self->base.data)[0x4d + i];
    /* tint + alpha = black (one lv.q/sv.q quad copy) */
    spr->tint[0] = g_colorBlack.x;
    spr->tint[1] = g_colorBlack.y;
    spr->tint[2] = g_colorBlack.z;
    spr->alpha = g_colorBlack.w;
    ((GfxSprite **)self->base.data)[1]->posZ -= 1.0f;
  }
  ((GfxSprite **)self->base.data)[0x4d]->posX -= 2.0f;
  ((GfxSprite **)self->base.data)[0x4d]->posY -= 2.0f;
  ((GfxSprite **)self->base.data)[0x4e]->posY -= 2.0f;
  ((GfxSprite **)self->base.data)[0x4f]->posX += 2.0f;
  ((GfxSprite **)self->base.data)[0x4f]->posY -= 2.0f;
  ((GfxSprite **)self->base.data)[0x50]->posX -= 2.0f;
  ((GfxSprite **)self->base.data)[0x51]->posX += 2.0f;
  ((GfxSprite **)self->base.data)[0x52]->posX -= 2.0f;
  ((GfxSprite **)self->base.data)[0x52]->posY += 2.0f;
  ((GfxSprite **)self->base.data)[0x53]->posY += 2.0f;
  ((GfxSprite **)self->base.data)[0x54]->posX += 2.0f;
  ((GfxSprite **)self->base.data)[0x54]->posY += 2.0f;

  for (i = 0; i < 8; i++) {
    ((GfxSprite **)self->base.data)[2]->flags |= 1;
    MemLock();
    wasLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    spr = MemAlloc(0x160, (char *)0x0, 0);
    MemSetAllocFromLow(wasLow);
    MemUnlock();
    copy = NULL;
    if (spr != NULL) {
      GfxSpriteCtor(spr);
      copy = spr;
    }
    ((GfxSprite **)self->base.data)[0x55 + i] = copy;
    GfxSpriteLayerAdd(self->base.spriteLayer,
                      (CoreObject *)((GfxSprite **)self->base.data)[0x55 + i]);
    GfxSpriteCopy(((GfxSprite **)self->base.data)[2],
                  ((GfxSprite **)self->base.data)[0x55 + i]);
    spr = ((GfxSprite **)self->base.data)[0x55 + i];
    /* tint + alpha = black (one lv.q/sv.q quad copy) */
    spr->tint[0] = g_colorBlack.x;
    spr->tint[1] = g_colorBlack.y;
    spr->tint[2] = g_colorBlack.z;
    spr->alpha = g_colorBlack.w;
    ((GfxSprite **)self->base.data)[2]->posZ -= 1.0f;
  }
  ((GfxSprite **)self->base.data)[0x55]->posX -= 2.0f;
  ((GfxSprite **)self->base.data)[0x55]->posY -= 2.0f;
  ((GfxSprite **)self->base.data)[0x56]->posY -= 2.0f;
  ((GfxSprite **)self->base.data)[0x57]->posX += 2.0f;
  ((GfxSprite **)self->base.data)[0x57]->posY -= 2.0f;
  ((GfxSprite **)self->base.data)[0x58]->posX -= 2.0f;
  ((GfxSprite **)self->base.data)[0x59]->posX += 2.0f;
  ((GfxSprite **)self->base.data)[0x5a]->posX -= 2.0f;
  ((GfxSprite **)self->base.data)[0x5a]->posY += 2.0f;
  ((GfxSprite **)self->base.data)[0x5b]->posY += 2.0f;
  ((GfxSprite **)self->base.data)[0x5c]->posX += 2.0f;
  ((GfxSprite **)self->base.data)[0x5c]->posY += 2.0f;

  GfxSpriteSetVCell((float)(self->combo - 1), ((GfxSprite **)self->base.data)[4]);
  ((GfxSprite **)self->base.data)[4]->posY = 215.0f;
  for (i = 0; i < 8; i++) {
    ((GfxSprite **)self->base.data)[4]->flags |= 1;
    MemLock();
    wasLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    spr = MemAlloc(0x160, (char *)0x0, 0);
    MemSetAllocFromLow(wasLow);
    MemUnlock();
    copy = NULL;
    if (spr != NULL) {
      GfxSpriteCtor(spr);
      copy = spr;
    }
    ((GfxSprite **)self->base.data)[0x5d + i] = copy;
    GfxSpriteLayerAdd(self->base.spriteLayer,
                      (CoreObject *)((GfxSprite **)self->base.data)[0x5d + i]);
    GfxSpriteCopy(((GfxSprite **)self->base.data)[4],
                  ((GfxSprite **)self->base.data)[0x5d + i]);
    spr = ((GfxSprite **)self->base.data)[0x5d + i];
    /* tint + alpha = black (one lv.q/sv.q quad copy) */
    spr->tint[0] = g_colorBlack.x;
    spr->tint[1] = g_colorBlack.y;
    spr->tint[2] = g_colorBlack.z;
    spr->alpha = g_colorBlack.w;
    ((GfxSprite **)self->base.data)[4]->posZ -= 1.0f;
  }
  ((GfxSprite **)self->base.data)[0x5d]->posX -= 2.0f;
  ((GfxSprite **)self->base.data)[0x5d]->posY -= 2.0f;
  ((GfxSprite **)self->base.data)[0x5e]->posY -= 2.0f;
  ((GfxSprite **)self->base.data)[0x5f]->posX += 2.0f;
  ((GfxSprite **)self->base.data)[0x5f]->posY -= 2.0f;
  ((GfxSprite **)self->base.data)[0x60]->posX -= 2.0f;
  ((GfxSprite **)self->base.data)[0x61]->posX += 2.0f;
  ((GfxSprite **)self->base.data)[0x62]->posX -= 2.0f;
  ((GfxSprite **)self->base.data)[0x62]->posY += 2.0f;
  ((GfxSprite **)self->base.data)[0x63]->posY += 2.0f;
  ((GfxSprite **)self->base.data)[0x64]->posX += 2.0f;
  ((GfxSprite **)self->base.data)[0x64]->posY += 2.0f;

  GfxSpriteLayerSetFlagsAll(self->base.spriteLayer, 0x20);
  UiComboListUpdateSlots(self, self->combo);
  for (i = 5; i < 0xb; i++) {
    ((GfxSprite **)self->base.data)[i]->flags |= 1;
  }
  for (i = 0xd; i < 0x19; i++) {
    ((GfxSprite **)self->base.data)[i]->flags |= 1;
  }
  if (self->combo == 1 || self->combo == 3 || self->combo == 4 || self->combo == 6 ||
      self->combo == 0xe || self->combo == 0xf || self->combo == 0x10 || self->combo == 0x12) {
    for (i = 0x19; i < 0x1f; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
    }
  }
  else {
    spr = ((GfxSprite **)self->base.data)[0x35];
    x = spr->posX;
    width = GfxSpriteGetWidth(spr);
    spr->posX = x + (width * 1.5f - 2.0f);
    spr = ((GfxSprite **)self->base.data)[0x39];
    x = spr->posX;
    width = GfxSpriteGetWidth(spr);
    spr->posX = x + (width * 1.5f - 2.0f);
  }
  GfxSpriteSetVCell(1.0f, ((GfxSprite **)self->base.data)[0x45]);
  ((GfxSprite **)self->base.data)[0x45]->flags |= 1;
  ((GfxSprite **)self->base.data)[0x49]->alpha = 0.5f;
  ((GfxSprite **)self->base.data)[0x49]->flags |= 1;
  ((GfxSprite **)self->base.data)[0x4a]->posY = 272.0f;
  ((GfxSprite **)self->base.data)[0x4a]->alpha = 0.5f;
  ((GfxSprite **)self->base.data)[0x4a]->flags |= 1;
  GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[0x4b]);
  GfxSpriteResetMatrix(((GfxSprite **)self->base.data)[0x4b]);
  ((GfxSprite **)self->base.data)[0x4b]->alpha = 0.5f;
  ((GfxSprite **)self->base.data)[0x4b]->flags |= 1;
  for (i = 0x47; i < 0x49; i++) {
    ((GfxSprite **)self->base.data)[i]->flags |= 1;
  }
  return;
}
