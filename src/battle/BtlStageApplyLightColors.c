// bdc 0x0889d4d4 BtlStageApplyLightColors
#include "bdc.h"

/* Copies the current arena's main light colour (`color`, `+0xc`) to the model's `color` (`+0x70`)
   and its ambient colour (`ambient`, `+0x30`) to the model's `ambient` (`+0x60`)
   (`BtlStageGetLightInfo` with arena -1), both with alpha 1.0. `obj` is a `GfxModel` (or an
   object derived from it); called for arena models, stage objects, items and the demos. */

void BtlStageApplyLightColors(void *obj)
{
  GfxModel *model = (GfxModel *)obj;
  BtlStageLight *light = BtlStageGetLightInfo(-1);

  model->color[0] = light->color[0];
  model->color[1] = light->color[1];
  model->color[2] = light->color[2];
  model->color[3] = 1.0f;
  model->ambient[0] = light->ambient[0];
  model->ambient[1] = light->ambient[1];
  model->ambient[2] = light->ambient[2];
  model->ambient[3] = 1.0f;
}
