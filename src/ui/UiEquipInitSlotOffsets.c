// bdc 0x089592ec UiEquipInitSlotOffsets
#include "bdc.h"

/* For each player of `UiEquip`, centres `count` sprites starting at index `first`
   (advancing by `count` per player), enables linear filtering and stores their offsets from the
   player's anchor sprite (`base`, advancing by `stride`) in the table `spritePos`. */

void UiEquipInitSlotOffsets(UiEquip *self, u16 base, u16 stride, u16 first, u16 count)
{
  int player;
  int i;
  u16 anchor;
  u16 slot;

  anchor = base;
  for (player = 0; player < self->playerCount; player++) {
    for (i = 0; i < (int)count; i++) {
      slot = (u16)(first + i);
      GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[slot]);
      ((GfxSprite **)self->base.data)[slot]->flags |= 0x20;
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[slot], 1.0f, 1.0f, 0.0f);
      self->spritePos[slot][0] =
          ((GfxSprite **)self->base.data)[slot]->posX - self->spritePos[anchor][0];
      self->spritePos[slot][1] =
          ((GfxSprite **)self->base.data)[slot]->posY - self->spritePos[anchor][1];
    }
    anchor = (u16)(anchor + stride);
    first = (u16)(first + count);
  }
}
