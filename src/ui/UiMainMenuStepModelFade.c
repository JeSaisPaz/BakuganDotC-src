// bdc 0x089a8e7c UiMainMenuStepModelFade
#include "bdc.h"

/* Steps the model alpha fade (8 frames, `modelFadeT` += 1/8) of the base model and the five item
   models (`ambient[3]` of each). Opening (`closing == 0`) eases in: alpha = start + (1 - (t-1)^2);
   closing fades out: alpha = start - t^2, where start is `baseAlpha` / `items[i].alpha`. Once
   `modelFadeT` reaches 1 every model's alpha is pinned to 1 (opening) or 0 (closing) and 1 is
   returned; otherwise 0. Missing item models are skipped. */

u8 UiMainMenuStepModelFade(UiMainMenu *self, u8 closing)
{
  float t;
  float d;
  int i;

  t = self->modelFadeT + 0.125f;
  if (closing == 0) {
    self->modelFadeT = t;
    d = t - 1.0f;
    ((GfxModel *)self->baseModel)->ambient[3] = self->baseAlpha + (1.0f - d * d);
    for (i = 0; i < 5; i++) {
      if (self->models[i] != NULL) {
        d = self->modelFadeT - 1.0f;
        ((GfxModel *)self->models[i])->ambient[3] = self->items[i].alpha + (1.0f - d * d);
      }
    }
    if (self->modelFadeT < 1.0f) {
      return 0;
    }
    ((GfxModel *)self->baseModel)->ambient[3] = 1.0f;
    for (i = 0; i < 5; i++) {
      if (self->models[i] != NULL) {
        ((GfxModel *)self->models[i])->ambient[3] = 1.0f;
      }
    }
    return 1;
  }
  self->modelFadeT = t;
  ((GfxModel *)self->baseModel)->ambient[3] = self->baseAlpha - t * t;
  for (i = 0; i < 5; i++) {
    if (self->models[i] != NULL) {
      t = self->modelFadeT;
      ((GfxModel *)self->models[i])->ambient[3] = self->items[i].alpha - t * t;
    }
  }
  if (self->modelFadeT < 1.0f) {
    return 0;
  }
  ((GfxModel *)self->baseModel)->ambient[3] = 0.0f;
  for (i = 0; i < 5; i++) {
    if (self->models[i] != NULL) {
      ((GfxModel *)self->models[i])->ambient[3] = 0.0f;
    }
  }
  return 1;
}
