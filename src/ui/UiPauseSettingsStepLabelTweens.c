// bdc 0x089ac7fc UiPauseSettingsStepLabelTweens
#include "bdc.h"

/* Steps the four row-label tweens (`labelTweens`, sprites 27..30) by one frame; returns true once at least
 * one of them has finished (UiTweenUpdate returns true on completion). */

int UiPauseSettingsStepLabelTweens(UiPauseSettings *self, u8 closing)
{
  u8 finished = 0;
  int i;

  for (i = 0x1b; i < 0x1f; i++) {
    finished += UiTweenUpdate(1.0f, 1.5f, 16.0f, closing, ((GfxSprite **)self->base.data)[i],
                          &self->labelTweens[i - 0x1b], 1);
  }
  return finished != 0;
}
