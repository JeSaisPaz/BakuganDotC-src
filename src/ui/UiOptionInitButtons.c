// bdc 0x08970d1c UiOptionInitButtons
#include "bdc.h"

/* Shows the OK and Defaults button sprites 0x30/0x31 of `UiOption`. */

void UiOptionInitButtons(UiOption *self)

{
  int i;

  for (i = 48; i < 50; i++) {
    UiOptionShowSprite(&self->base, ((GfxSprite **)self->base.data)[i]);
    ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
    self->spriteY[i] = ((GfxSprite **)self->base.data)[i]->posY;
  }
}
