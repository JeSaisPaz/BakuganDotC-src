// bdc 0x08918730 UiAdvSelectLoadPedestal
#include "bdc.h"

/* Loads the pedestal model `menu_daiza.gmo` of the adventure partner-select screen
   (`UiAdvSelectCtor`, task 376): a 0x140-byte `GfxModel` allocated from low
   memory and stored in `pedestal` (NULL when the allocation fails), sets a grey specular
   (0.6, power 8, `GfxModelSetSpecular`), scales its root matrix to 0.45, places it at
   (0, -40) and copies `pos` into the root matrix translation row (w = 1). */

void UiAdvSelectLoadPedestal(UiAdvSelect *self)
{
  bool fromLow;
  GfxModel *model;
  GmoModel *gmo;
  float specular[4] __attribute__((aligned(16)));

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  model = MemAlloc(sizeof(GfxModel), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (model != NULL) {
    GfxModelCtor(model, "menu_daiza.gmo", 0);
  }
  self->pedestal = model;
  specular[0] = 0.6f;
  specular[1] = 0.6f;
  specular[2] = 0.6f;
  specular[3] = 1.0f;
  GfxModelSetSpecular(8.0f, model, specular, NULL);
  gmo = self->pedestal->data;
  gmo->rootMatrix[10] = 0.45f;
  gmo->rootMatrix[5] = 0.45f;
  gmo->rootMatrix[0] = 0.45f;
  self->pedestal->pos[0] = 0.0f;
  self->pedestal->pos[1] = -40.0f;
  model = self->pedestal;
  /* quad copy of pos into the translation row (lv.q/sv.q) */
  model->data->rootMatrix[12] = model->pos[0];
  model->data->rootMatrix[13] = model->pos[1];
  model->data->rootMatrix[14] = model->pos[2];
  model->data->rootMatrix[15] = model->pos[3];
  self->pedestal->data->rootMatrix[15] = 1.0f;
}
