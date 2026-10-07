// bdc 0x088a95d4 ActorStageObjGetBounds
#include "bdc.h"

/* Returns the model's bounding box (`model+0x130 → +0x38`: `{float min[4], max[4]}`) or, when the
   model has none, the static unit box `0x08af7fe0` (-1..1). `box[1]` (min Y) is the ground offset
   used when placing objects. */

float *ActorStageObjGetBounds(ActorStageObjBase *self)

{
  float *bbox;

  bbox = ((self->base).data)->bbox;
  if (bbox == (float *)0x0) {
    g_stageObjUnitBounds[0] = -1.0f;
    g_stageObjUnitBounds[1] = -1.0f;
    g_stageObjUnitBounds[2] = -1.0f;
    g_stageObjUnitBounds[3] = 0.0f;
    g_stageObjUnitBounds[7] = 0.0f;
    g_stageObjUnitBounds[4] = 1.0f;
    g_stageObjUnitBounds[5] = 1.0f;
    g_stageObjUnitBounds[6] = 1.0f;
    return g_stageObjUnitBounds;
  }
  return bbox;
}
