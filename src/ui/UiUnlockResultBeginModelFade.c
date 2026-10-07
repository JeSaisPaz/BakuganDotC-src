// bdc 0x08939c10 UiUnlockResultBeginModelFade
#include "bdc.h"

/* Starts the fade of the reward model `+0x794` of `UiUnlockResult` (t `+0x7a0`
   = 0): opening clears its alpha (`+0x6c`) and sets the colour to 0.8 grey; closing records the
   current alpha in `+0x7a4`. */

void UiUnlockResultBeginModelFade(UiUnlockResult *self, u8 closing)

{
  GfxModel *model;

  if (closing == 0) {
    self->modelTimer = 0.0f;
    model = (GfxModel *)self->model;
    model->ambient[3] = 0.0f;
    model = (GfxModel *)self->model;
    self->modelFadeFrom = 0.0f;
    model->scale[3] = 0.0f;
    model->scale[0] = 0.8f;
    model->scale[1] = 0.8f;
    model->scale[2] = 0.8f;
    return;
  }
  self->modelTimer = 0.0f;
  self->modelFadeFrom = ((GfxModel *)self->model)->ambient[3];
  return;
}
