// bdc 0x0895914c UiEquipInitPanelOffsets
#include "bdc.h"

/* Centres four panel sprites of `UiEquip` (indices `+0x5162`, `+0x516e`, `+0x51aa`,
   `+0x51fa`) and stores two vertical offsets between them (`+0x5158`, `+0x515c`). */

void UiEquipInitPanelOffsets(UiEquip *self)
{
  GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[self->spriteIdx[1]]);
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->spriteIdx[1]], 1.0f, 1.0f, 0.0f);
  GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[self->spriteIdx[7]]);
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->spriteIdx[7]], 1.0f, 1.0f, 0.0f);
  GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[self->spriteIdx[0x25]]);
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->spriteIdx[0x25]], 1.0f, 1.0f, 0.0f);
  GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[self->spriteIdx[0x4d]]);
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->spriteIdx[0x4d]], 1.0f, 1.0f, 0.0f);

  GfxSprite **sprites = (GfxSprite **)self->base.data;
  self->rowGapA = sprites[self->spriteIdx[1]]->posY - sprites[self->spriteIdx[7]]->posY;
  self->rowGapB = sprites[self->spriteIdx[0x25]]->posY - sprites[self->spriteIdx[0x4d]]->posY;
}
