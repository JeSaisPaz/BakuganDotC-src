// bdc 0x08970dc4 UiOptionInitButtonLabels
#include "bdc.h"

/* Shows the button label sprites 0x32/0x33 of `UiOption`. */

void UiOptionInitButtonLabels(UiOption *self)

{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  int i;

  for (i = 0x32; i < 0x34; i++) {
    UiOptionShowSprite(&self->base, sprites[i]);
    sprites[i]->alpha = 1.0f;
    self->spriteY[i] = sprites[i]->posY;
  }
}
