// bdc 0x089ada90 UiPauseSettingsStepHelpIcons
#include "bdc.h"

/* Steps the appear (`closing == 0`) or disappear tween of the four button-help icons of
   `UiPauseSettings` (sprites 57..60, `data+0xe4..0xf0`, tweens `helpIconTweens`, 16
   frames, alpha only) with `UiTweenUpdate`; returns true once a tween reports finished. */

bool UiPauseSettingsStepHelpIcons(UiPauseSettings *self, u8 closing)
{
  u8 done = 0;
  int i;

  for (i = 0x39; i < 0x3d; i++) {
    done += UiTweenUpdate(1.0f, 1.0f, 16.0f, closing, ((GfxSprite **)self->base.data)[i],
                          &self->helpIconTweens[i - 0x39], 1);
  }
  return done != 0;
}
