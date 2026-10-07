// bdc 0x08970510 UiOptionStartSlide
#include "bdc.h"

/* Resets the vertical slide of `UiOption` (`+0xb9c..0xba8`): opening starts 272 px
   below, closing at 0; moves all 0x3a sprites to their saved Y plus the offset and calls
   `UiEquipSetPendingFlag(1)` for each. */

void UiOptionStartSlide(UiOption *self, u8 closing)

{
  GfxSprite **sprites;
  int i;

  if (closing == 0) {
    self->slideT = 0.0f;
    self->slideOffset = 272.0f;
    self->slideStart = 272.0f;
    self->slideBounced = 0;
  } else {
    self->slideOffset = 0.0f;
    self->slideStart = 0.0f;
    self->slideT = 0.0f;
    self->slideBounced = 0;
  }
  for (i = 0; i < 0x3a; i++) {
    UiEquipSetPendingFlag(1);
    sprites = (GfxSprite **)self->base.data;
    sprites[i]->posY = self->spriteY[i] + self->slideOffset;
  }
}
