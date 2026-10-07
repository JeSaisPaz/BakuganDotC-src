// bdc 0x089a7318 UiMainMenuBobModels
#include "bdc.h"

/* While `bobbing` is set, bobs each item model (except item 1) up and down by up to 10 units with a
   cosine (period 90 frames) and copies its position to the attached object `data` (the root matrix
   translation, w forced to 1). Run at the start of every phase handler. */

void UiMainMenuBobModels(UiMainMenu *self)

{
  int i;

  if (self->bobbing != 0) {
    for (i = 0; i < 5; i++) {
      if (i != 1) {
        GfxModel *model;
        GmoModel *data;
        float c;
        float t = self->bob[i][2] + 0.022222223f;
        self->bob[i][2] = t;
        c = __builtin_cosf(t * 3.1415927f);
        model = (GfxModel *)self->models[i];
        model->pos[1] = self->bob[i][1] + (1.0f - c) * 0.5f * 10.0f;
        model = (GfxModel *)self->models[i];
        data = model->data;
        data->rootMatrix[12] = model->pos[0];
        data->rootMatrix[13] = model->pos[1];
        data->rootMatrix[14] = model->pos[2];
        data->rootMatrix[15] = model->pos[3];
        ((GfxModel *)self->models[i])->data->rootMatrix[15] = 1.0f;
      }
    }
  }
}
