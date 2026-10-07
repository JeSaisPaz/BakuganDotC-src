// bdc 0x08932fc0 UiGauntletSetupLoadBakuganModel
#include "bdc.h"

/* Loads the model of the player's partner Bakugan for the gauntlet setup screen
   (`UiGauntletSetup`, task 373): clears `modelScratch` and `motionName`, takes the id from the
   save profile (`curBakugan & 0xff`), resolves its `*_stay_sel` motion
   (`UiBakuganGetSelectMotionName`) into `motionName`, allocates a 0x140-byte `GfxModel` from
   the low end of the heap and constructs it from `g_btlModelNames``[id]` (`GfxModelCtor`;
   `bakuganModel` stays NULL when the allocation fails and is still used), enables motions, loads
   `"<motion>.gmo"` and plays the motion looped with blend 0.2 (`GfxModelPlayMotion`), runs vtable
   slot 7, stores the id in `base.unk08`, sets up the shading (`BtlBakuganSetupModelShading`),
   sets the root matrix diagonal to `UiBakuganGetModelScale` * `UiBakuganGetModelOffset``(id, 0)`
   * 0.6, rotates it (`UiBakuganRotateModel`), places the model at (0, -40) and copies `pos` into
   the matrix translation row (w = 1), sets the motion speed to 0.5 (vtable slot 6) and sets
   `ambient[3]` to 1. */

void UiGauntletSetupLoadBakuganModel(UiGauntletSetup *self)
{
  char fileName[72];
  GfxModel *model;
  GfxModel *alloc;
  float *matrix;
  const VtblEntry *slot;
  bool fromLow;
  u32 id;
  float scale;

  memset(self->modelScratch, 0, sizeof(self->modelScratch));
  memset(self->motionName, 0, sizeof(self->motionName));
  id = SaveGetProfile()->data->curBakugan & 0xff;
  UiBakuganGetSelectMotionName(id, self->motionName);

  model = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  alloc = MemAlloc(0x140, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (alloc != NULL) {
    GfxModelCtor(alloc, g_btlModelNames[id], 0);
    model = alloc;
  }
  self->bakuganModel = model;
  GfxModelEnableMotion(model);

  sprintf(fileName, "%s.gmo", self->motionName);
  GmoMotionLoadFile(GmoMotionMgrGet(), fileName);
  model = self->bakuganModel;
  GfxModelPlayMotion(0.2f, model, GmoMotionIndexOfName(GmoMotionMgrGet(), self->motionName), 1);

  model = self->bakuganModel;
  slot = &((const VtblEntry *)model->base.vtable)[7];
  ((void (*)(void *))slot->fn)((u8 *)model + slot->delta);

  ((GfxModel *)self->bakuganModel)->base.unk08 = id;
  BtlBakuganSetupModelShading(self->bakuganModel);

  matrix = ((GfxModel *)self->bakuganModel)->data->rootMatrix;
  scale = UiBakuganGetModelScale(id, 0);
  scale = scale * UiBakuganGetModelOffset(id, 0) * 0.6f;
  matrix[10] = scale;
  matrix[5] = scale;
  matrix[0] = scale;
  UiBakuganRotateModel(self->bakuganModel, id, 0);

  ((GfxModel *)self->bakuganModel)->pos[0] = 0.0f;
  ((GfxModel *)self->bakuganModel)->pos[1] = -40.0f;
  model = self->bakuganModel;
  matrix = model->data->rootMatrix;
  matrix[12] = model->pos[0];
  matrix[13] = model->pos[1];
  matrix[14] = model->pos[2];
  matrix[15] = model->pos[3];
  ((GfxModel *)self->bakuganModel)->data->rootMatrix[15] = 1.0f;

  model = self->bakuganModel;
  slot = &((const VtblEntry *)model->base.vtable)[6];
  ((void (*)(float, void *))slot->fn)(0.5f, (u8 *)model + slot->delta);
  ((GfxModel *)self->bakuganModel)->ambient[3] = 1.0f;
}
