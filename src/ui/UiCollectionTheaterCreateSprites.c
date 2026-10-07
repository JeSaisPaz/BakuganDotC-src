// bdc 0x08987fac UiCollectionTheaterCreateSprites
#include "bdc.h"

/* Builds the sprites of `UiCollectionTheater`: clears the tween records
   (`+0x74`, 0x7a8 bytes), creates layout 0x2a (`UiLayoutCreateSprites`, 0x30 sprites), hides,
   centres and zero-alphas them saving their depths (`spriteZ`, `thumbZ`, `spriteZTail`: one
   0x30-entry table), adds a copy of the cursor sprite 6 as the highlight sprite 0x30 and saves the
   positions of the thumbnail sprites 13..18 in `thumbPos`. */

void UiCollectionTheaterCreateSprites(UiScreen *screen)
{
  UiCollectionTheater *self = (UiCollectionTheater *)screen;
  GfxSprite **sprites;
  float *savedZ;
  GfxSprite *sprite;
  GfxSprite *highlight;
  bool fromLow;
  u32 i;

  /* Everything from just past unk70 up to spriteZ (+0x74..+0x81b): the tween records. */
  memset(&self->unk70 + 1, 0, (u8 *)self->spriteZ - (u8 *)(&self->unk70 + 1));
  UiLayoutCreateSprites(self->base.spriteLayer, self->base.data, 0x2a);

  /* spriteZ[13], thumbZ[6] and spriteZTail[29] are one contiguous table of 0x30 depths. */
  savedZ = self->spriteZ;
  for (i = 0; i < 0x30; i++) {
    sprites = (GfxSprite **)self->base.data;
    sprites[i]->flags &= ~1u;
    GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[i]);
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->alpha = 0.0f;
    savedZ[i] = ((GfxSprite **)self->base.data)[i]->posZ;
  }

  highlight = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  sprite = MemAlloc(sizeof(GfxSprite), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (sprite != NULL) {
    GfxSpriteCtor(sprite);
    highlight = sprite;
  }
  ((GfxSprite **)self->base.data)[48] = highlight;
  GfxSpriteLayerAdd(self->base.spriteLayer, (CoreObject *)((GfxSprite **)self->base.data)[48]);
  GfxSpriteCopy(((GfxSprite **)self->base.data)[6], ((GfxSprite **)self->base.data)[48]);
  ((GfxSprite **)self->base.data)[48]->flags &= ~1u;

  sprites = (GfxSprite **)self->base.data;
  for (i = 0; i < 6; i++) {
    self->thumbPos[i][0] = sprites[13 + i]->posX;
    self->thumbPos[i][1] = sprites[13 + i]->posY;
  }
}
