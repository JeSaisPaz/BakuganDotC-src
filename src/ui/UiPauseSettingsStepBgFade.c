// bdc 0x089ac184 UiPauseSettingsStepBgFade
#include "bdc.h"

/* Steps the background-panel fade (16 frames, eased); hides the panel at the end of the closing
   fade. Returns 1 when done, else 0. */

int UiPauseSettingsStepBgFade(UiPauseSettings *self, u8 closing)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  float t = self->bgFadeT + 0.0625f;

  if (closing == 0) {
    self->bgFadeT = t;
    sprites[0x19]->alpha = self->bgFadeFrom + (1.0f - (t - 1.0f) * (t - 1.0f));
    t = self->bgFadeT - 1.0f;
    self->bgAlpha = self->bgFadeFrom + (1.0f - t * t);
    if (!(self->bgFadeT < 1.0f)) {
      ((GfxSprite **)self->base.data)[0x19]->alpha = 1.0f;
      self->bgAlpha = 1.0f;
      return 1;
    }
  } else {
    self->bgFadeT = t;
    sprites[0x19]->alpha = self->bgFadeFrom - t * t;
    t = self->bgFadeT;
    self->bgAlpha = self->bgFadeFrom - t * t;
    if (!(t < 1.0f)) {
      GfxSprite *s = ((GfxSprite **)self->base.data)[0x19];
      s->flags &= ~1u;
      return 1;
    }
  }
  return 0;
}
