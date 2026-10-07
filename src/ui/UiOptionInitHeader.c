// bdc 0x08970828 UiOptionInitHeader
#include "bdc.h"

/* Shows sprite 0x1a of `UiOption` (header panel) and saves its Y position. */

void UiOptionInitHeader(UiOption *self)

{
  int i;

  for (i = 26; i < 27; i++) {
    UiOptionShowSprite(&self->base, ((GfxSprite **)self->base.data)[i]);
    ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
    self->spriteY[i] = ((GfxSprite **)self->base.data)[i]->posY;
  }
}
