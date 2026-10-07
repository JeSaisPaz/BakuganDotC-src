// bdc 0x0892c9d8 UiBakuganSelectCreateSprites
#include "bdc.h"

/* Creates the layout sprites (layout 0x2f, `UiLayoutCreateSprites`) of the Bakugan select screen
   (`UiBakuganSelectCtor`, task 371) into the sprite table `base.data`, after clearing all tweens.
   Sprites 0..131 are hidden (flags bit0 cleared), centred, scale 1 / angle 0, alpha 0, and their
   positions are recorded in `spriteZ` / `spritePos`. Sprite 132 is a new heap sprite added to the
   sprite layer (`GfxSpriteLayerAdd`) as a hidden copy of sprite 25 (`GfxSpriteCopy`). Then the
   name panel is measured (`UiBakuganSelectMeasureNamePanel`) and `layerOffset` records the
   positions of sprites 94, 114 and 115 relative to sprite 26. */

void UiBakuganSelectCreateSprites(UiBakuganSelect *self)
{
  bool fromLow;
  GfxSprite *sprite;
  GfxSprite *copy;
  GfxSprite **sprites;
  u32 i;

  memset(self->tweens, 0, sizeof(self->tweens));
  UiLayoutCreateSprites((self->base).spriteLayer, (self->base).data, 0x2f);
  for (i = 0; i < 0x84; i++) {
    sprites = (GfxSprite **)(self->base).data;
    sprites[i]->flags &= ~1u;
    GfxSpriteCenterPivot(((GfxSprite **)(self->base).data)[i]);
    UiSpriteSetScaleRotation(((GfxSprite **)(self->base).data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)(self->base).data)[i]->alpha = 0.0f;
    sprites = (GfxSprite **)(self->base).data;
    self->spriteZ[i] = sprites[i]->posZ;
    self->spritePos[i][0] = sprites[i]->posX;
    self->spritePos[i][1] = sprites[i]->posY;
  }

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  sprite = MemAlloc(sizeof(GfxSprite), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  copy = NULL;
  if (sprite != NULL) {
    GfxSpriteCtor(sprite);
    copy = sprite;
  }
  ((GfxSprite **)(self->base).data)[0x84] = copy;
  GfxSpriteLayerAdd((self->base).spriteLayer,
                    (CoreObject *)((GfxSprite **)(self->base).data)[0x84]);
  sprites = (GfxSprite **)(self->base).data;
  GfxSpriteCopy(sprites[25], sprites[0x84]);
  ((GfxSprite **)(self->base).data)[0x84]->flags &= ~1u;
  UiBakuganSelectMeasureNamePanel(self);

  sprites = (GfxSprite **)(self->base).data;
  self->layerOffset[0][0] = sprites[94]->posX - sprites[26]->posX;
  self->layerOffset[0][1] = sprites[94]->posY - sprites[26]->posY;
  self->layerOffset[1][0] = sprites[114]->posX - sprites[26]->posX;
  self->layerOffset[1][1] = sprites[114]->posY - sprites[26]->posY;
  self->layerOffset[2][0] = sprites[115]->posX - sprites[26]->posX;
  self->layerOffset[2][1] = sprites[115]->posY - sprites[26]->posY;
}
