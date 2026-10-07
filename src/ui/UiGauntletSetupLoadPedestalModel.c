// bdc 0x08933228 UiGauntletSetupLoadPedestalModel
#include "bdc.h"

/* Creates the pedestal model of `UiGauntletSetup`: a 0x140-byte
   `GfxModel` for `"menu_daiza.gmo"` (allocated from low memory) stored in
   `pedestalModel` (NULL if the allocation failed — the code below then dereferences it anyway),
   specular colour (0.6, 0.6, 0.6, 1) power 8 (`GfxModelSetSpecular`), root matrix scale 0.27,
   position (0, −40) copied into the root matrix translation row, whose w is then set to 1. */

void UiGauntletSetupLoadPedestalModel(UiGauntletSetup *self)
{
  GfxModel *model = NULL;
  GfxModel *mem;
  GmoModel *data;
  bool fromLow;
  float specular[4] BDC_ALIGN16;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0x140, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != NULL) {
    GfxModelCtor(mem, "menu_daiza.gmo", 0);
    model = mem;
  }
  self->pedestalModel = model;

  specular[0] = 0.6f;
  specular[1] = 0.6f;
  specular[2] = 0.6f;
  specular[3] = 1.0f;
  GfxModelSetSpecular(8.0f, model, specular, NULL);

  data = ((GfxModel *)self->pedestalModel)->data;
  data->rootMatrix[10] = 0.27f;
  data->rootMatrix[5] = 0.27f;
  data->rootMatrix[0] = 0.27f;
  ((GfxModel *)self->pedestalModel)->pos[0] = 0.0f;
  ((GfxModel *)self->pedestalModel)->pos[1] = -40.0f;

  model = self->pedestalModel;
  data = model->data;
  data->rootMatrix[12] = model->pos[0];
  data->rootMatrix[13] = model->pos[1];
  data->rootMatrix[14] = model->pos[2];
  data->rootMatrix[15] = model->pos[3];
  ((GfxModel *)self->pedestalModel)->data->rootMatrix[15] = 1.0f;
}
