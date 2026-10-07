// bdc 0x08939ff8 UiUnlockResultUpdateModelFade
#include "bdc.h"

/* Advances the reward-model fade of `UiUnlockResult` by 1/8 (`modelTimer`):
   opening raises the model's ambient alpha from `modelFadeFrom` with an overshoot curve
   (1 - (t-1)²) and eases `scale` x/y/z from `UiUnlockResultGetModelBrightness` of `rewardIndex`
   + 0.6 down to that value; closing lowers the alpha by t² and grows `scale` by 0.6·t. `scale[3]`
   is zeroed each step. Returns false while `modelTimer < 1`; otherwise snaps the final state
   (opening: alpha 1 and the target value; closing: alpha 0) and returns true. */

bool UiUnlockResultUpdateModelFade(UiUnlockResult *self, u8 closing)

{
  GfxModel *model;
  float t;
  float v;

  t = self->modelTimer + 0.125f;
  if (closing == 0) {
    self->modelTimer = t;
    ((GfxModel *)self->model)->ambient[3] = self->modelFadeFrom + (1.0f - (t - 1.0f) * (t - 1.0f));
    model = (GfxModel *)self->model;
    v = UiUnlockResultGetModelBrightness(self, (u8)self->rewardIndex);
    model->scale[3] = 0.0f;
    v = (v + 0.6f) - self->modelTimer * 0.6f;
    model->scale[0] = v;
    model->scale[1] = v;
    model->scale[2] = v;
    if (self->modelTimer < 1.0f) {
      return false;
    }
    ((GfxModel *)self->model)->ambient[3] = 1.0f;
    model = (GfxModel *)self->model;
    v = UiUnlockResultGetModelBrightness(self, (u8)self->rewardIndex);
    model->scale[0] = v;
    model->scale[1] = v;
    model->scale[2] = v;
    model->scale[3] = 0.0f;
  }
  else {
    self->modelTimer = t;
    ((GfxModel *)self->model)->ambient[3] = self->modelFadeFrom - t * t;
    model = (GfxModel *)self->model;
    v = UiUnlockResultGetModelBrightness(self, (u8)self->rewardIndex);
    model->scale[3] = 0.0f;
    v = v + self->modelTimer * 0.6f;
    model->scale[0] = v;
    model->scale[1] = v;
    model->scale[2] = v;
    if (self->modelTimer < 1.0f) {
      return false;
    }
    ((GfxModel *)self->model)->ambient[3] = 0.0f;
  }
  return true;
}
