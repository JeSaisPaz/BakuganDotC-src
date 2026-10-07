// bdc 0x089184a4 UiAdvSelectLoadBakuganModel
#include "bdc.h"

/* Loads the 3D model of the Bakugan under the cursor of the adventure partner-select screen
   (`UiAdvSelectCtor`, task 376): clears `modelScratch` and `motionName`, resolves the candidate's
   `*_stay_sel` motion (`UiBakuganGetSelectMotionName`) into `motionName`, allocates a 0x140-byte
   `GfxModel` from the low end of the heap and constructs it from `g_btlModelNames``[id]`
   (`GfxModelCtor`; `model` stays NULL when the allocation fails and is still used), enables
   motions, loads `"<motion>.gmo"` and plays the motion looped with blend 0.2
   (`GfxModelPlayMotion`), runs vtable slot 7 (motion update), stores the id in `base.unk08`, sets
   up the shading (`BtlBakuganSetupModelShading`), scales the root matrix diagonal by
   `UiBakuganGetModelScale` * `UiBakuganGetModelOffset``(id, 0)`, rotates it
   (`UiBakuganRotateModel`), places the model at (0, -40) and copies `pos` into the matrix
   translation row (w = 1), sets the motion speed to 0.5 (vtable slot 6) and sets `ambient[3]` to
   1. */

void UiAdvSelectLoadBakuganModel(UiAdvSelect *self)
{
  char fileName[72];
  GfxModel *model;
  GfxModel *alloc;
  float *matrix;
  const VtblEntry *slot;
  bool fromLow;
  float scale;

  memset(self->modelScratch, 0, sizeof(self->modelScratch));
  memset(self->motionName, 0, sizeof(self->motionName));
  UiBakuganGetSelectMotionName(self->candidates[self->cursor].bakugan, self->motionName);

  model = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  alloc = MemAlloc(0x140, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (alloc != NULL) {
    GfxModelCtor(alloc, g_btlModelNames[self->candidates[self->cursor].bakugan], 0);
    model = alloc;
  }
  self->model = model;
  GfxModelEnableMotion(model);

  sprintf(fileName, "%s.gmo", self->motionName);
  GmoMotionLoadFile(GmoMotionMgrGet(), fileName);
  model = self->model;
  GfxModelPlayMotion(0.2f, model, GmoMotionIndexOfName(GmoMotionMgrGet(), self->motionName), 1);

  model = self->model;
  slot = &((const VtblEntry *)model->base.vtable)[7];
  ((void (*)(void *))slot->fn)((u8 *)model + slot->delta);

  model = self->model;
  model->base.unk08 = self->candidates[self->cursor].bakugan;
  BtlBakuganSetupModelShading(self->model);

  matrix = ((GfxModel *)self->model)->data->rootMatrix;
  scale = UiBakuganGetModelScale(self->candidates[self->cursor].bakugan, 0);
  scale = scale * UiBakuganGetModelOffset(self->candidates[self->cursor].bakugan, 0);
  matrix[10] = scale;
  matrix[5] = scale;
  matrix[0] = scale;
  UiBakuganRotateModel(self->model, self->candidates[self->cursor].bakugan, 0);

  ((GfxModel *)self->model)->pos[0] = 0.0f;
  ((GfxModel *)self->model)->pos[1] = -40.0f;
  model = self->model;
  /* quad copy of pos into the translation row (lv.q/sv.q) */
  model->data->rootMatrix[12] = model->pos[0];
  model->data->rootMatrix[13] = model->pos[1];
  model->data->rootMatrix[14] = model->pos[2];
  model->data->rootMatrix[15] = model->pos[3];
  ((GfxModel *)self->model)->data->rootMatrix[15] = 1.0f;

  model = self->model;
  slot = &((const VtblEntry *)model->base.vtable)[6];
  ((float (*)(void *, float))slot->fn)((u8 *)model + slot->delta, 0.5f);
  ((GfxModel *)self->model)->ambient[3] = 1.0f;
}
