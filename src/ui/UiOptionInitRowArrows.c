// bdc 0x089708d0 UiOptionInitRowArrows
#include "bdc.h"

/* Shows the 24 arrow sprites of `UiOption` (1..0x18: three per side per row, left
   set 1..12, right set 13..24 mirrored with `GfxSpriteFlipU`), tinting every third one blue, and
   saves their Y positions. */

void UiOptionInitRowArrows(UiOption *self)
{
  int i;

  for (i = 1; i < 0x19; i++) {
    GfxSprite *sprite = ((GfxSprite **)self->base.data)[i];

    if ((i - 1) % 3 == 2) {
      sprite->tint[0] = 0.0f;
      sprite->tint[1] = 0.5f;
      sprite->tint[2] = 1.0f;
      sprite->alpha = 0.0f;
      sprite = ((GfxSprite **)self->base.data)[i];
    }
    UiOptionShowSprite(&self->base, sprite);
    ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
    self->spriteY[i] = ((GfxSprite **)self->base.data)[i]->posY;
    if (i - 1 >= 12) {
      GfxSpriteFlipU(((GfxSprite **)self->base.data)[i]);
    }
  }
}
