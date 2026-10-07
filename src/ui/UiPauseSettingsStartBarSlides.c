// bdc 0x089acfb4 UiPauseSettingsStartBarSlides
#include "bdc.h"

/* Starts the `barSlides` tweens of the volume bar/frame sprites 0x24..0x29. Opening (`closing` 0):
   the three bars 0x24..0x26 are first sized for their slider value
   (`UiPauseSettingsGetItemValue`, `UiPauseSettingsSetBarWidth`); every sprite is made visible
   and slides in from +32 px X with alpha (`UiTweenBeginSlide` flags 5). Closing: centres the
   pivot, flags 0x20, resets scale and slides out by 32 px (flags 7, alpha + scale + X). */

void UiPauseSettingsStartBarSlides(UiPauseSettings *self, u8 closing)
{
  s32 i;
  UiTween *tween;
  GfxSprite *bar;

  if (closing == 0) {
    tween = self->barSlides;
    for (i = 0x24; i < 0x2a; i++, tween++) {
      bar = ((GfxSprite **)self->base.data)[i];
      if (i >= 0x24 && i < 0x27) {
        UiPauseSettingsSetBarWidth(self, bar, UiPauseSettingsGetItemValue(self, (u8)(i - 0x24)));
        bar = ((GfxSprite **)self->base.data)[i];
      }
      bar->flags |= 1;
      UiTweenBeginSlide(1.0f, 32.0f, 0.0f, 0, ((GfxSprite **)self->base.data)[i], tween, 5);
    }
  } else {
    tween = self->barSlides;
    for (i = 0x24; i < 0x2a; i++, tween++) {
      GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[i]);
      bar = ((GfxSprite **)self->base.data)[i];
      bar->flags |= 0x20;
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
      UiTweenBeginSlide(1.0f, 0.0f, 32.0f, 1, ((GfxSprite **)self->base.data)[i], tween, 7);
    }
  }
}
