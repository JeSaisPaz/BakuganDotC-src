// bdc 0x089a8a80 UiMainMenuStartFrameFade
#include "bdc.h"

/* Prepares the fade of the frame sprite `data+4` and the base model's alpha (`frameAlpha`): opening
   shows the sprite and starts from 0, closing starts from 1. Stepped by
   `UiMainMenuStepFrameFade`. */

void UiMainMenuStartFrameFade(UiMainMenu *self, u8 closing)

{
  GfxSprite *sprite;

  if (closing == 0) {
    sprite = ((GfxSprite **)self->base.data)[1];
    self->frameAlpha = 0.0f;
    sprite->flags |= 1;
    self->slots[1].tween.t = 0.0f;
    self->slots[1].tween.startAlpha = 0.0f;
    return;
  }
  self->slots[1].tween.t = 0.0f;
  self->frameAlpha = 1.0f;
  self->slots[1].tween.startAlpha = 1.0f;
  return;
}
