// bdc 0x08913eac UiUpgradeLoadBakuganModel
#include "bdc.h"

/* Loads the 3D model and camera of the selected Bakugan (`bakugan`) on the upgrade screen
   (`maybe_UiScreen490Ctor`, task 490): allocates a 0x140-byte `GfxModel` from the low end of
   the heap and constructs it from `g_btlModelNames``[bakugan]` (`GfxModelCtor`; `model` stays
   NULL when the allocation fails and is still used), enables motions, formats the Bakugan-specific
   `"ability03_a"` motion name into `modelName` and `"stay_sel"` into `motionName`
   (`UiUpgradeFormatMotionName`), loads both `"<name>.gmo"` files (`GmoMotionLoadFile`), plays
   `stay_sel` looped with blend 0.2 (`GfxModelPlayMotion`), runs vtable slot 7, stores the id in
   `base.unk08`, sets up the shading (`BtlBakuganSetupModelShading`), scales the root matrix
   diagonal by `UiBakuganGetModelScale` * `UiBakuganGetModelOffset``(id, 0)` * 0.4, rotates it
   (`UiBakuganRotateModel`), places the model at (3, -60) and copies `pos` into the matrix
   translation row (w = 1), sets the motion speed to 0.5 (vtable slot 6) and sets `ambient[3]` to 1.
   Then allocates a 0x2a0-byte `GfxCamera` the same way (`GfxCameraCtor`, `GfxCameraInit`
   on the possibly NULL `camera`), aims it from (0, 0, 150) at the origin, sets the screen offset
   (-140, -90) (`GfxCameraSetScreenOffset`), fully updates it (`GfxCameraUpdate`) and runs its
   vtable slot 2. */

void UiUpgradeLoadBakuganModel(UiUpgrade *self)
{
  char fileName[72];
  GfxModel *model;
  GfxModel *alloc;
  GfxCamera *cam;
  GfxCamera *camAlloc;
  float *matrix;
  const VtblEntry *slot;
  bool fromLow;
  float scale;

  model = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  alloc = MemAlloc(sizeof(GfxModel), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (alloc != NULL) {
    GfxModelCtor(alloc, g_btlModelNames[self->bakugan], 0);
    model = alloc;
  }
  self->model = model;
  GfxModelEnableMotion(model);

  UiUpgradeFormatMotionName(self, self->modelName, "ability03_a", self->bakugan & 0xff);
  sprintf(fileName, "%s.gmo", self->modelName);
  GmoMotionLoadFile(GmoMotionMgrGet(), fileName);
  UiUpgradeFormatMotionName(self, self->motionName, "stay_sel", self->bakugan & 0xff);
  sprintf(fileName, "%s.gmo", self->motionName);
  GmoMotionLoadFile(GmoMotionMgrGet(), fileName);
  model = self->model;
  GfxModelPlayMotion(0.2f, model, GmoMotionIndexOfName(GmoMotionMgrGet(), self->motionName), 1);

  model = self->model;
  slot = &((const VtblEntry *)model->base.vtable)[7];
  ((void (*)(void *))slot->fn)((u8 *)model + slot->delta);

  model = self->model;
  model->base.unk08 = self->bakugan;
  BtlBakuganSetupModelShading(self->model);

  matrix = ((GfxModel *)self->model)->data->rootMatrix;
  scale = UiBakuganGetModelScale(self->bakugan & 0xff, 0);
  scale = scale * UiBakuganGetModelOffset(self->bakugan & 0xff, 0) * 0.4f;
  matrix[10] = scale;
  matrix[5] = scale;
  matrix[0] = scale;
  UiBakuganRotateModel(self->model, self->bakugan & 0xff, 0);

  ((GfxModel *)self->model)->pos[0] = 3.0f;
  ((GfxModel *)self->model)->pos[1] = -60.0f;
  model = self->model;
  /* copies pos (all four lanes) into the translation row */
  matrix = model->data->rootMatrix;
  matrix[12] = model->pos[0];
  matrix[13] = model->pos[1];
  matrix[14] = model->pos[2];
  matrix[15] = model->pos[3];
  ((GfxModel *)self->model)->data->rootMatrix[15] = 1.0f;

  model = self->model;
  slot = &((const VtblEntry *)model->base.vtable)[6];
  ((float (*)(void *, float))slot->fn)((u8 *)model + slot->delta, 0.5f);
  ((GfxModel *)self->model)->ambient[3] = 1.0f;

  cam = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  camAlloc = MemAlloc(sizeof(GfxCamera), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (camAlloc != NULL) {
    GfxCameraCtor(&camAlloc->base);
    cam = camAlloc;
  }
  self->camera = cam;
  GfxCameraInit(cam);

  cam = self->camera;
  cam->target[0] = 0.0f;
  cam->target[1] = 0.0f;
  cam->target[2] = 0.0f;
  cam->target[3] = 0.0f;
  cam = self->camera;
  cam->eye[0] = 0.0f;
  cam->eye[1] = 0.0f;
  cam->eye[2] = 150.0f;
  cam->eye[3] = 0.0f;
  GfxCameraSetScreenOffset(-140.0f, -90.0f, self->camera);
  GfxCameraUpdate(self->camera, 0xffffffff);
  cam = self->camera;
  slot = &((const VtblEntry *)cam->base.vtable)[2];
  ((void (*)(void *))slot->fn)((u8 *)cam + slot->delta);
}
