// bdc 0x089add54 UiPauseSettingsStepToggle
#include "bdc.h"

/* Steps the toggle-value sprite tween `toggleTween` (sprite 0x3d) of `UiPauseSettings`
   started by `UiPauseSettingsBeginToggle` (`UiTweenUpdate`, 16 frames; opening scale 1.0 with
   flags 5, closing scale 1.0 -> 1.5 with flags 7); returns true when it has finished. */

bool UiPauseSettingsStepToggle(UiPauseSettings *self, u8 closing)
{
  u8 done;
  int i;

  done = 0;
  if (closing == 0) {
    for (i = 0x3d; i < 0x3e; i++) {
      done += UiTweenUpdate(1.0f, 1.0f, 16.0f, 0, ((GfxSprite **)self->base.data)[i],
                            &(&self->toggleTween)[i - 0x3d], 5);
    }
  } else {
    for (i = 0x3d; i < 0x3e; i++) {
      done += UiTweenUpdate(1.0f, 1.5f, 16.0f, 1, ((GfxSprite **)self->base.data)[i],
                            &(&self->toggleTween)[i - 0x3d], 7);
    }
  }
  return done != 0;
}
