// bdc 0x08934510 UiGauntletSetupOkButtonDone
#include "bdc.h"

/* Advances the OK-button tweens of `UiGauntletSetup` (sprites 0x26 and 0x2f,
   see `UiGauntletSetupTweenOkButton`) with `UiTweenUpdate` (scale 1.0 -> 1.0, 16 frames,
   mode 1). The u8 sum of the per-tween results is nonzero when any tween reported true, so the
   function returns true once either tween reports done (UiTweenUpdate's result). */

bool UiGauntletSetupOkButtonDone(UiGauntletSetup *self, u8 closing)
{
  u8 done = 0;
  s32 i;

  for (i = 0x26; i < 0x27; i++) {
    done += UiTweenUpdate(1.0f, 1.0f, 16.0f, closing, ((GfxSprite **)self->base.data)[i],
                          &self->tweens[i], 1);
  }
  for (i = 0x2f; i < 0x30; i++) {
    done += UiTweenUpdate(1.0f, 1.0f, 16.0f, closing, ((GfxSprite **)self->base.data)[i],
                          &self->tweens[i], 1);
  }
  return done != 0;
}
