// bdc 0x089ac8d4 UiPauseSettingsStartArrowSlides
#include "bdc.h"

/* Opening (`closing == 0`): shows the eight value arrows (sprites 0x2a..0x31), slides them in from
   32 px to the left (`UiTweenBeginSlide`, `arrowTweens` at `+0x708`) and greys the advisor-toggle
   arrows (sprites 0x2d and 0x31: tint 0.5, alpha 0) when `UiPauseSettingsAdviceAvailable` is
   false. Closing: slides the same arrows out 32 px to the left with a fade-out. */

void UiPauseSettingsStartArrowSlides(UiPauseSettings *self, u8 closing)
{
  GfxSprite *sprite;
  UiTween *tween;
  int i;

  if (closing == 0) {
    tween = self->arrowTweens;
    for (i = 0x2a; i < 0x32; i++, tween++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      UiTweenBeginSlide(1.0f, -32.0f, 0.0f, 0, ((GfxSprite **)self->base.data)[i], tween, 7);
      if (i < 0x2e) {
        if (i < 0x2d)
          continue;
      } else if (i != 0x31) {
        continue;
      }
      if (UiPauseSettingsAdviceAvailable(self) == 0) {
        sprite = ((GfxSprite **)self->base.data)[i];
        sprite->tint[0] = 0.5f;
        sprite->tint[1] = 0.5f;
        sprite->tint[2] = 0.5f;
        sprite->alpha = 0.0f;
      }
    }
  } else {
    tween = self->arrowTweens;
    for (i = 0x2a; i < 0x32; i++, tween++)
      UiTweenBeginSlide(1.0f, 0.0f, -32.0f, 1, ((GfxSprite **)self->base.data)[i], tween, 7);
  }
}
