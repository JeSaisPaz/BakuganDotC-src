// bdc 0x089570c0 UiEquipLoadPedestalModel
#include "bdc.h"

/* Creates the `"menu_daiza.gmo"` pedestal model of player slot `slot` of `UiEquip`
   (0x140-byte `GfxModel` allocated from low memory, stored in `pedestalModels[slot]`),
   with specular 0.6 grey power 8 (`GfxModelSetSpecular`), a uniform root scale of 0.45 (up to 2
   players) or 0.3 (more), and its position shifted by `UiEquipGetPedestalOffset` and copied into the
   root matrix translation row (w = 1). */

void UiEquipLoadPedestalModel(UiEquip *self, u8 slot)
{
  bool fromLow;
  GfxModel *model;
  GmoModel *gmo;
  float specular[4];
  float offset[2];

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  model = MemAlloc(0x140, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (model != NULL) {
    GfxModelCtor(model, "menu_daiza.gmo", 0);
  }
  self->pedestalModels[slot] = model;
  specular[0] = 0.6f;
  specular[1] = 0.6f;
  specular[2] = 0.6f;
  specular[3] = 1.0f;
  GfxModelSetSpecular(8.0f, model, specular, NULL);
  if (self->playerCount < 3) {
    gmo = self->pedestalModels[slot]->data;
    gmo->rootMatrix[10] = 0.45f;
    gmo->rootMatrix[5] = 0.45f;
    gmo->rootMatrix[0] = 0.45f;
  } else {
    gmo = self->pedestalModels[slot]->data;
    gmo->rootMatrix[10] = 0.3f;
    gmo->rootMatrix[5] = 0.3f;
    gmo->rootMatrix[0] = 0.3f;
  }
  UiEquipGetPedestalOffset(offset, &self->base, slot);
  self->pedestalModels[slot]->pos[0] += offset[0];
  self->pedestalModels[slot]->pos[1] += offset[1];
  model = self->pedestalModels[slot];
  model->data->rootMatrix[12] = model->pos[0];
  model->data->rootMatrix[13] = model->pos[1];
  model->data->rootMatrix[14] = model->pos[2];
  model->data->rootMatrix[15] = model->pos[3];
  self->pedestalModels[slot]->data->rootMatrix[15] = 1.0f;
}
