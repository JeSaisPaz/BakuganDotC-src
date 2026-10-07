// bdc 0x0892ce28 UiBakuganSelectLoadModel
#include "bdc.h"

/* Loads the model of the current entry (`current`) of the Bakugan select screen
   (`UiBakuganSelectCtor`, task 371): clears `modelScratch` and `motionName`, resolves the
   entry's `*_stay_sel` motion (`UiBakuganGetSelectMotionName`) into `motionName`, allocates a
   0x140-byte `GfxModel` from the low end of the heap and constructs it from
   `g_btlModelNames``[id]` (`GfxModelCtor`; `model` stays NULL when the allocation fails and is
   still used), enables motions, loads `"<motion>.gmo"` and plays the motion looped with blend 0.2
   (`GfxModelPlayMotion`), runs vtable slot 7 (`GfxModelUpdateAndApplyMotion`), stores the id in
   `base.unk08`, sets up the shading (`BtlBakuganSetupModelShading`), scales the root matrix
   diagonal by `UiBakuganGetModelScale` * `UiBakuganGetModelOffset``(id, 0)`, rotates it
   (`UiBakuganRotateModel`), places the model at (0, -40) and copies `pos` into the matrix
   translation row (w = 1), sets the motion speed to 0.5 (vtable slot 6, `GfxModelSetMotionSpeed`)
   and clears `ambient[3]`. */

void UiBakuganSelectLoadModel(UiBakuganSelect *self)
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
  UiBakuganGetSelectMotionName(self->entries[self->current].bakugan, self->motionName);

  model = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  alloc = MemAlloc(0x140, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (alloc != NULL) {
    GfxModelCtor(alloc, g_btlModelNames[self->entries[self->current].bakugan], 0);
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
  model->base.unk08 = self->entries[self->current].bakugan;
  BtlBakuganSetupModelShading(self->model);

  matrix = ((GfxModel *)self->model)->data->rootMatrix;
  scale = UiBakuganGetModelScale(self->entries[self->current].bakugan, 0);
  scale = scale * UiBakuganGetModelOffset(self->entries[self->current].bakugan, 0);
  matrix[10] = scale;
  matrix[5] = scale;
  matrix[0] = scale;
  UiBakuganRotateModel(self->model, self->entries[self->current].bakugan, 0);

  ((GfxModel *)self->model)->pos[0] = 0.0f;
  ((GfxModel *)self->model)->pos[1] = -40.0f;
  model = self->model;
  model->data->rootMatrix[12] = model->pos[0];
  model->data->rootMatrix[13] = model->pos[1];
  model->data->rootMatrix[14] = model->pos[2];
  model->data->rootMatrix[15] = model->pos[3];
  ((GfxModel *)self->model)->data->rootMatrix[15] = 1.0f;

  model = self->model;
  slot = &((const VtblEntry *)model->base.vtable)[6];
  ((float (*)(void *, float))slot->fn)((u8 *)model + slot->delta, 0.5f);
  ((GfxModel *)self->model)->ambient[3] = 0.0f;
}
