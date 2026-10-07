// bdc 0x08971730 UiOptionHideButtonGlow
#include "bdc.h"

/* Hides the button glow sprite 0x34 and the highlight copy 0x3a of `UiOption`. */

void UiOptionHideButtonGlow(UiOption *self)

{
  GfxSprite **sprites = (GfxSprite **)self->base.data;

  sprites[0x34]->flags &= ~1u;
  sprites[0x3a]->flags &= ~1u;
  return;
}
