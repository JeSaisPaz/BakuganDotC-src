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

  dx = VfSub(unit->pos[0], self->base.base.pos[0]);
  dy = 0.0f;
  dz = VfSub(unit->pos[2], self->base.base.pos[2]);
  dist = VfSqrt(VfDot3(dx, dy, dz, dx, dy, dz));
  if (!(dist <= self->auraRadius)) {
    return 0;
  }
  return 1;
}
