// bdc 0x0888ead4 BtlAiDistanceToUnit
#include "bdc.h"

/* Returns the 3D distance between the owner of `BtlAi` (`ai+0x1a0`, position `+0x20`)
   and `unit` (the current target `ai+0x96c` when `unit` is NULL), or -1.0 when there is no unit. */

float BtlAiDistanceToUnit(BtlAi *self, void *unit)
{
  BtlBakugan *target = unit;
  const float *a;
  const float *b;
  float dx, dy, dz;

  if (target == NULL) {
    target = self->target;
  }
  if (target == NULL) {
    return -1.0f;
  }
  /* |owner->pos - target->pos| over x, y, z */
  a = self->owner->base.pos;
  b = target->base.pos;
  dx = a[0] - b[0];
  dy = a[1] - b[1];
  dz = a[2] - b[2];
  return __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
}
