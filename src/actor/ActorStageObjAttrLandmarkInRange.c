// bdc 0x088a79c0 ActorStageObjAttrLandmarkInRange
#include "bdc.h"

/* Returns 1 when unit `unit` is within the aura radius `auraRadius` of the landmark (horizontal XZ
   distance, y of the difference zeroed), else 0 (also 0 when the distance is NaN). */

int ActorStageObjAttrLandmarkInRange(ActorStageObjAttrLandmark *self, GfxModel *unit)
{
  float dx;
  float dy;
  float dz;
  float dist;

  dx = unit->pos[0] - self->base.base.pos[0];
  dy = 0.0f;
  dz = unit->pos[2] - self->base.base.pos[2];
  dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
  if (!(dist <= self->auraRadius)) {
    return 0;
  }
  return 1;
}
