// bdc 0x0893bf7c UiUnlockResultShowButtonPrompt
#include "bdc.h"

/* Shows (`visible`: button icon 2 via `UiSetButtonIcon`, alpha 1) or hides the confirm-button prompt
   sprites 0x21 and 0x22 of `UiUnlockResult`. */

void UiUnlockResultShowButtonPrompt(UiUnlockResult *self, u8 visible)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;

  if (visible == 0) {
    sprites[0x21]->flags &= ~1u;
    ((GfxSprite **)self->base.data)[0x22]->flags &= ~1u;
  } else {
    UiSetButtonIcon(sprites[0x21], 2);
    ((GfxSprite **)self->base.data)[0x21]->flags |= 1;
    ((GfxSprite **)self->base.data)[0x22]->flags |= 1;
    ((GfxSprite **)self->base.data)[0x21]->alpha = 1.0f;
    ((GfxSprite **)self->base.data)[0x22]->alpha = 1.0f;
  }
}
