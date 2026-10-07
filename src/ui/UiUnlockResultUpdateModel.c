// bdc 0x0893b830 UiUnlockResultUpdateModel
#include "bdc.h"

/* Per-frame update of the reward model `+0x794` of `UiUnlockResult`: bobs it
   vertically (Y = base `+0x7e8` + 0.16·sin(angle)), advancing the angle `+0x7a0` by 2°
   (kind 5) or 4° (kind 6) and wrapping at 360°, then runs the model's virtual update (vtable
   entry 7: adjustor + function).
   The `vsin.s` of angle·(2/π) (bank S703) is sin(angle in radians). */

void UiUnlockResultUpdateModel(UiUnlockResult *self)
{
  GfxModel *model = (GfxModel *)self->model;
  const VtblEntry *entry;

  if (model == NULL) {
    return;
  }
  model->rot[1] = self->modelBaseY +
                  __builtin_sinf(self->modelTimer * 3.1415927f * 0.0055555557f) * 0.16f;
  model = (GfxModel *)self->model;
  if (!(self->modelTimer < 360.0f)) {
    self->modelTimer = 0.0f;
  }
  if (self->rewardKind == 5) {
    self->modelTimer = self->modelTimer + 2.0f;
  } else if (self->rewardKind == 6) {
    self->modelTimer = self->modelTimer + 4.0f;
  }
  entry = &((const VtblEntry *)model->base.vtable)[7];
  ((void (*)(void *))entry->fn)((u8 *)model + entry->delta);
}
