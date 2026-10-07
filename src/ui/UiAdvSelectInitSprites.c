// bdc 0x08918048 UiAdvSelectInitSprites
#include "bdc.h"

/* Initial sprite setup of the adventure partner-select screen (task 376, `UiAdvSelectCtor`,
   class prefix `UiAdvSelect`; sprites `adv_baku_%02d`, `adv_chara_%02d`, `adv_yaji_*`,
   `main_bg.fab`; cursor `+0x74`, candidate ids at `+0x8a0` (4 bytes per slot)): clears the work
   area `+0x78..+0x6b8`, builds the screen's sprites from layout table 0x32 (`UiLayoutCreateSprites`), for each of
   the first 0x27 sprites clears flags bit0, centres the pivot, sets unit scale and zero alpha while recording their base positions
   (`+0x6b8` depth, `+0x758`/`+0x75c` x/y), then creates sprite 0x27 (0x160 bytes, low heap) as a copy of sprite 0x11,
   adds it to the layer, measures the name panels and stores sprite 0x12 minus sprite 5 position
   in `candidateOffsetX/Y`. */

void UiAdvSelectInitSprites(UiAdvSelect *self)
{
  bool fromLow;
  GfxSprite *sprite;
  GfxSprite **sprites;
  u32 i;

  memset(self->tweens, 0, sizeof(self->tweens));
  UiLayoutCreateSprites(self->base.spriteLayer, (GfxSprite **)self->base.data, 0x32);
  for (i = 0; i < 0x27; i++) {
    ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
    GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[i]);
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->alpha = 0.0f;
    sprites = (GfxSprite **)self->base.data;
    self->spriteZ[i] = sprites[i]->posZ;
    self->spritePos[i][0] = sprites[i]->posX;
    self->spritePos[i][1] = sprites[i]->posY;
  }
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  sprite = (GfxSprite *)MemAlloc(sizeof(GfxSprite), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (sprite != NULL) {
    GfxSpriteCtor(sprite);
  }
  ((GfxSprite **)self->base.data)[0x27] = sprite;
  GfxSpriteLayerAdd(self->base.spriteLayer, (CoreObject *)((GfxSprite **)self->base.data)[0x27]);
  sprites = (GfxSprite **)self->base.data;
  GfxSpriteCopy(sprites[0x11], sprites[0x27]);
  ((GfxSprite **)self->base.data)[0x27]->flags &= ~1u;
  UiAdvSelectMeasureNamePanels(self);
  sprites = (GfxSprite **)self->base.data;
  self->candidateOffsetX = sprites[0x12]->posX - sprites[5]->posX;
  self->candidateOffsetY = sprites[0x12]->posY - sprites[5]->posY;
}
