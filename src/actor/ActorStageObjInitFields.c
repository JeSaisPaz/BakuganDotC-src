// bdc 0x088aa39c ActorStageObjInitFields
#include "bdc.h"

/* Clears the stage-object pointers (`+0x140..+0x168`, `hpGauge`, `buffer`), picks the root node
   (node 1, else 0, `GfxModelGetNode`), zeroes `restPos`, `extents` and the four `uvScrolls`
   (from the VFPU zero bank C720), clears HP and flags, and derives the size fields from the model
   bounds (`ActorStageObjGetBounds`): `halfHeight` = (max - min).y / 2, `diagonal` = |min - max|
   (xyz), `diagonal2` = the same with both y set to 0, `extents` = max - min when min.x < 0, else
   max + min (y of both already 0, w from max). Ends with `recordArg` = 0. Called by
   `ActorStageObjBaseCtor` and `ActorStageObjBaseCtorByName`. */

void ActorStageObjInitFields(ActorStageObjBase *self)
{
  float max[4];
  float min[4];
  float *bounds;
  float dx;
  float dy;
  float dz;
  s32 i;

  self->triggeredBy = NULL;
  self->record = NULL;
  self->hitBy = NULL;
  self->ptr14c = NULL;
  self->hpGauge = NULL;
  self->soundParams = NULL;
  self->ptr150 = NULL;
  self->collider = NULL;
  self->lights = NULL;
  self->buffer = NULL;
  self->rootNode = GfxModelGetNode(&self->base, 1);
  if (self->rootNode == NULL) {
    self->rootNode = GfxModelGetNode(&self->base, 0);
  }
  self->restPos[0] = 0.0f;
  self->restPos[1] = 0.0f;
  self->restPos[2] = 0.0f;
  self->restPos[3] = 0.0f;
  self->hp = 0;
  self->maxHp = 0;
  self->baseAlpha = 1.0f;
  self->instanceId = 0;
  self->targeted = 0;
  self->fadeState = 0;
  self->visible = 0;

  bounds = ActorStageObjGetBounds(self);
  for (i = 0; i < 4; i++) {
    max[i] = bounds[4 + i];
  }
  bounds = ActorStageObjGetBounds(self);
  for (i = 0; i < 4; i++) {
    min[i] = bounds[i];
  }

  self->halfHeight = (max[1] - min[1]) * 0.5f;

  dx = min[0] - max[0];
  dy = min[1] - max[1];
  dz = min[2] - max[2];
  self->diagonal = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);

  max[1] = 0.0f;
  min[1] = 0.0f;
  dx = min[0] - max[0];
  dy = min[1] - max[1];
  dz = min[2] - max[2];
  self->diagonal2 = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);

  self->extents[0] = 0.0f;
  self->extents[1] = 0.0f;
  self->extents[2] = 0.0f;
  self->extents[3] = 0.0f;
  if (min[0] < 0.0f) {
    self->extents[0] = max[0] - min[0];
    self->extents[1] = max[1] - min[1];
    self->extents[2] = max[2] - min[2];
  } else {
    self->extents[0] = max[0] + min[0];
    self->extents[1] = max[1] + min[1];
    self->extents[2] = max[2] + min[2];
  }
  self->extents[3] = max[3];

  for (i = 0; i < 64; i++) {
    self->uvScrolls[i] = 0;
  }
  self->recordArg = 0;
}
