// bdc 0x089a8acc UiMainMenuStepFrameFade
#include "bdc.h"

/* Steps the fade started by `UiMainMenuStartFrameFade` (eased, 8 frames): sprite `data[1]` alpha
   and `frameAlpha` go up (opening) or down (closing; the sprite is hidden at the end). Returns 1
   when finished. */

int UiMainMenuStepFrameFade(UiMainMenu *self, u8 closing)

{
  float start = self->slots[1].tween.startAlpha;
  float t = self->slots[1].tween.t + 0.125f;
  GfxSprite *sprite = ((GfxSprite **)self->base.data)[1];
  float d;

  if (closing == 0) {
    self->slots[1].tween.t = t;
    d = t - 1.0f;
    sprite->alpha = start + (1.0f - d * d);
    t = self->slots[1].tween.t;
    d = t - 1.0f;
    self->frameAlpha = self->slots[1].tween.startAlpha + (1.0f - d * d);
    if (!(t < 1.0f)) {
      ((GfxSprite **)self->base.data)[1]->alpha = 1.0f;
      return 1;
    }
  } else {
    self->slots[1].tween.t = t;
    sprite->alpha = start - t * t;
    t = self->slots[1].tween.t;
    d = t - 1.0f;
    self->frameAlpha = self->slots[1].tween.startAlpha - (1.0f - d * d);
    if (!(t < 1.0f)) {
      ((GfxSprite **)self->base.data)[1]->flags &= ~1u;
      return 1;
    }
  }
  return 0;
}
