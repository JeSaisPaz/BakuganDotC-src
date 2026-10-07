// bdc 0x08958bc0 UiEquipInitGridOffsets
#include "bdc.h"

/* Centres the grid base sprite (`spriteIdx[0]`) and the 20 Bakugan face sprites (`spriteIdx[1] + i`)
   of `UiEquip`, resets their scale to 1 and rotation to 0, and stores each face's
   position relative to the base in `faceOffset[i]`; does the same for the cursor sprite
   (`spriteIdx[5]`, `cursorOffset`). */

void UiEquipInitGridOffsets(UiEquip *self)

{
  GfxSprite **sprites;
  int i;

  sprites = (GfxSprite **)self->base.data;
  GfxSpriteCenterPivot(sprites[self->spriteIdx[0]]);
  sprites = (GfxSprite **)self->base.data;
  UiSpriteSetScaleRotation(sprites[self->spriteIdx[0]], 1.0f, 1.0f, 0.0f);
  for (i = 0; i < 20; i++) {
    sprites = (GfxSprite **)self->base.data;
    GfxSpriteCenterPivot(sprites[self->spriteIdx[1] + i]);
    sprites = (GfxSprite **)self->base.data;
    UiSpriteSetScaleRotation(sprites[self->spriteIdx[1] + i], 1.0f, 1.0f, 0.0f);
    sprites = (GfxSprite **)self->base.data;
    self->faceOffset[i][0] =
        sprites[self->spriteIdx[1] + i]->posX - sprites[self->spriteIdx[0]]->posX;
    self->faceOffset[i][1] =
        sprites[self->spriteIdx[1] + i]->posY - sprites[self->spriteIdx[0]]->posY;
  }
  GfxSpriteCenterPivot(sprites[self->spriteIdx[5]]);
  sprites = (GfxSprite **)self->base.data;
  UiSpriteSetScaleRotation(sprites[self->spriteIdx[5]], 1.0f, 1.0f, 0.0f);
  sprites = (GfxSprite **)self->base.data;
  self->cursorOffset[0] = sprites[self->spriteIdx[5]]->posX - sprites[self->spriteIdx[0]]->posX;
  self->cursorOffset[1] = sprites[self->spriteIdx[5]]->posY - sprites[self->spriteIdx[0]]->posY;
  return;
}
