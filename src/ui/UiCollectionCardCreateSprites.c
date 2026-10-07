// bdc 0x08982b80 UiCollectionCardCreateSprites
#include "bdc.h"

/* Creates the sprites of `UiCollectionCard` (card collection screen, task
   313): clears the tweens, instantiates layout 0x28 (`UiLayoutCreateSprites`) into the sprite
   table `base.data`, hides/centres/zero-alphas sprites 0..64 saving their depths in `spriteZ`,
   insets the UVs of sprites 0..4, builds sprite 65 as a hidden copy of sprite 4, and turns the four
   card sprites 13..16 into 128x176 `"card_L_001"` art (`GfxFindTexture`, `UiSpriteSetSize`,
   `GfxSpriteSetUvRectXYWH`) re-centred at their original position, saved in `bobPos`. */

void UiCollectionCardCreateSprites(UiCollectionCard *self)
{
  GfxSprite **sprites;
  GfxSprite *sprite;
  GfxSprite *copy;
  GfxSprite *mem;
  bool fromLow;
  float uvRect[4];
  float savedX;
  float savedY;
  u32 i;

  memset(self->tweens, 0, 0xa50);
  UiLayoutCreateSprites(self->base.spriteLayer, (GfxSprite **)self->base.data, 0x28);

  for (i = 0; i < 65; i++) {
    sprites = (GfxSprite **)self->base.data;
    sprites[i]->flags &= ~1u;
    GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[i]);
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->alpha = 0.0f;
    self->spriteZ[i] = ((GfxSprite **)self->base.data)[i]->posZ;
  }
  for (i = 0; i < 4; i++) {
    GfxSpriteInsetUv(0.5f, ((GfxSprite **)self->base.data)[i]);
  }
  for (i = 4; i < 5; i++) {
    GfxSpriteInsetUv(0.5f, ((GfxSprite **)self->base.data)[i]);
  }

  copy = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(GfxSprite), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != NULL) {
    GfxSpriteCtor(mem);
    copy = mem;
  }
  ((GfxSprite **)self->base.data)[65] = copy;
  GfxSpriteLayerAdd(self->base.spriteLayer, (CoreObject *)((GfxSprite **)self->base.data)[65]);
  sprites = (GfxSprite **)self->base.data;
  GfxSpriteCopy(sprites[4], sprites[65]);
  ((GfxSprite **)self->base.data)[65]->flags &= ~1u;

  for (i = 13; i < 17; i++) {
    sprite = ((GfxSprite **)self->base.data)[i];
    savedX = sprite->posX;
    savedY = sprite->posY;
    sprite->texture = GfxFindTexture("card_L_001");
    UiSpriteSetSize(128.0f, 176.0f, ((GfxSprite **)self->base.data)[i]);
    uvRect[0] = 0.0f;
    uvRect[1] = 0.0f;
    uvRect[2] = 128.0f;
    uvRect[3] = 176.0f;
    GfxSpriteSetUvRectXYWH(((GfxSprite **)self->base.data)[i], uvRect);
    GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[i]);
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posX = savedX;
    ((GfxSprite **)self->base.data)[i]->posY = savedY;
    GfxSpriteInsetUv(0.5f, ((GfxSprite **)self->base.data)[i]);
    sprites = (GfxSprite **)self->base.data;
    self->bobPos[i - 13][0] = sprites[i]->posX;
    self->bobPos[i - 13][1] = sprites[i]->posY;
  }
}
