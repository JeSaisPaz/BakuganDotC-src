// bdc 0x089ac694 UiPauseSettingsStepTitleTween
#include "bdc.h"

/* Steps the title sprite tween (`titleTween`, +0x488); returns 1 when done. */

int UiPauseSettingsStepTitleTween(UiPauseSettings *self, u8 closing)
{
  GfxSprite *sprite = ((GfxSprite **)self->base.data)[0x1a];

  if (closing == 0) {
    if (UiTweenUpdate(1.5f, 1.0f, 16.0f, 0, sprite, &self->titleTween, 3)) {
      return 1;
    }
  } else {
    if (UiTweenUpdate(1.0f, 1.5f, 16.0f, closing, sprite, &self->titleTween, 3)) {
      return 1;
    }
  }
  return 0;
}
