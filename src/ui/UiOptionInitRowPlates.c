// bdc 0x08970a9c UiOptionInitRowPlates
#include "bdc.h"

/* Shows the row plate sprites 0x1f..0x22 of `UiOption` (the cursor pulse targets of
   rows 0..3) and saves their Y positions. */

void UiOptionInitRowPlates(UiOption *self)

{
  int i;

  for (i = 31; i < 35; i++) {
    UiOptionShowSprite(&self->base, ((GfxSprite **)self->base.data)[i]);
    ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
    self->spriteY[i] = ((GfxSprite **)self->base.data)[i]->posY;
  }
}
