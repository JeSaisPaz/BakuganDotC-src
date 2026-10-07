// bdc 0x089acd6c UiPauseSettingsStepToggleSlides
#include "bdc.h"

/* Steps the four plate slides `plateSlides` (sprites 31..34 of `data`) by one frame with
   `UiTweenUpdate` over 16 frames: opening at scale 1.0 with flags 5, closing (fade-out) from 1.0
   to 1.5 with flags 7. Returns 1 once any of the four tweens has
   finished (`UiTweenUpdate` returns true when done), else 0. */

int UiPauseSettingsStepToggleSlides(UiPauseSettings *self, u8 closing)
{
  u8 finished;
  int i;

  finished = 0;
  if (closing == 0) {
    for (i = 31; i < 35; i++) {
      finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, 0, ((GfxSprite **)self->base.data)[i],
                               &self->plateSlides[i - 31], 5);
    }
  } else {
    for (i = 31; i < 35; i++) {
      finished += UiTweenUpdate(1.0f, 1.5f, 16.0f, 1, ((GfxSprite **)self->base.data)[i],
                               &self->plateSlides[i - 31], 7);
    }
  }
  return finished != 0;
}
