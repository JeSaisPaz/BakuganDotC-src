// bdc 0x089ad160 UiPauseSettingsStepBarSlides
#include "bdc.h"

/* Steps the six volume bar/frame slides `barSlides` (sprites 0x24..0x29 of `data`) by one frame with
   `UiTweenUpdate` over 16 frames: opening at scale 1.0 with flags 5, closing (fade-out) from 1.0
   to 1.5 with flags 7. Returns 1 when any of the tweens reported finished (they run in lockstep),
   else 0. */

int UiPauseSettingsStepBarSlides(UiPauseSettings *self, u8 closing)
{
  u8 done;
  int i;

  done = 0;
  if (closing == 0) {
    for (i = 0x24; i < 0x2a; i++) {
      done += UiTweenUpdate(1.0f, 1.0f, 16.0f, 0, ((GfxSprite **)self->base.data)[i],
                            &self->barSlides[i - 0x24], 5);
    }
  } else {
    for (i = 0x24; i < 0x2a; i++) {
      done += UiTweenUpdate(1.0f, 1.5f, 16.0f, 1, ((GfxSprite **)self->base.data)[i],
                            &self->barSlides[i - 0x24], 7);
    }
  }
  return done != 0;
}
