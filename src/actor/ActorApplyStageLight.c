// bdc 0x088d4080 ActorApplyStageLight
#include "bdc.h"

/* Copies the current stage's two light colours from the lighting table
   (`GameGetStageLight``(-1)`) into the actor: the vec3 at row `+0x0c` goes to the actor's `color`, the
   vec3 at row `+0x30` to its `ambient`, both with `w = 1.0`. */

void ActorApplyStageLight(void *actor)
{
  GfxModel *model = (GfxModel *)actor;
  const float *row = (const float *)GameGetStageLight(-1);

  model->color[0] = row[0xc / 4];
  model->color[1] = row[0x10 / 4];
  model->color[2] = row[0x14 / 4];
  model->color[3] = 1.0f;
  model->ambient[0] = row[0x30 / 4];
  model->ambient[1] = row[0x34 / 4];
  model->ambient[2] = row[0x38 / 4];
  model->ambient[3] = 1.0f;
}
