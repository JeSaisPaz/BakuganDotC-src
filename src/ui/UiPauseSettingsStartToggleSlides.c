// bdc 0x089acc30 UiPauseSettingsStartToggleSlides
#include "bdc.h"

/* Starts the X slides (`plateSlides`, flags 7, scale 1.0) of the four toggle/value plates (sprites
   0x1f..0x22 of `data`). Opening (`closing == 0`): sets each plate to the off texture
   (`UiPauseSettingsSetToggleTexture`), sets sprite flag bit 0 and slides it in from +32 px to 0;
   closing: slides it out from 0 to +32 px. */

void UiPauseSettingsStartToggleSlides(UiPauseSettings *self, u8 closing)

{
  UiTween *tween;
  s32 i;

  if (closing == 0) {
    tween = self->plateSlides;
    for (i = 0x1f; i < 0x23; i++) {
      UiPauseSettingsSetToggleTexture(self, ((GfxSprite **)self->base.data)[i], 0);
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      UiTweenBeginSlide(1.0f, 32.0f, 0.0f, 0, ((GfxSprite **)self->base.data)[i], tween, 7);
      tween++;
    }
  }
  else {
    tween = self->plateSlides;
    for (i = 0x1f; i < 0x23; i++) {
      UiTweenBeginSlide(1.0f, 0.0f, 32.0f, 0, ((GfxSprite **)self->base.data)[i], tween, 7);
      tween++;
    }
  }
  return;
}
