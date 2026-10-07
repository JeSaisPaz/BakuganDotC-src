// bdc 0x089aca64 UiPauseSettingsStepArrowSlides
#include "bdc.h"

/* Steps the eight arrow tweens `arrowTweens` (sprites 42..49 of `data`) by one frame with
   `UiTweenUpdate` over 16 frames: opening at scale 1.0 with flags 5, closing (fade-out) from 1.0
   to 1.5 with flags 7. Returns 1 while any tween is still running, else 0. */

int UiPauseSettingsStepArrowSlides(UiPauseSettings *self, u8 closing)
{
  u8 running;
  int i;

  running = 0;
  if (closing == 0) {
    for (i = 42; i < 50; i++) {
      running += UiTweenUpdate(1.0f, 1.0f, 16.0f, 0, ((GfxSprite **)self->base.data)[i],
                               &self->arrowTweens[i - 42], 5);
    }
  } else {
    for (i = 42; i < 50; i++) {
      running += UiTweenUpdate(1.0f, 1.5f, 16.0f, 1, ((GfxSprite **)self->base.data)[i],
                               &self->arrowTweens[i - 42], 7);
    }
  }
  return running != 0;
}
