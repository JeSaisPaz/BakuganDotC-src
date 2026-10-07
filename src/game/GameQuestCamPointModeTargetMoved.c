// bdc 0x088f8268 GameQuestCamPointModeTargetMoved
#include "bdc.h"

/* Returns 1 when the followed target's position (owner `+8`, its virtual slot 2) differs from the
   remembered position `targetPos` (squared xyz distance not below FLT_EPSILON, or NaN), else 0. */

typedef struct CamMovedOwner {
  void *unk0;
  const VtblEntry *vtbl; /* +0x04 */
} CamMovedOwner;

s32 GameQuestCamPointModeTargetMoved(GameQuestCamPointMode *self)
{
  CamMovedOwner *owner;
  const VtblEntry *e;
  const ScePspFVector4 *pos;
  float dx;
  float dy;
  float dz;
  float dist;

  owner = (CamMovedOwner *)self->base.base.base.followed;
  e = &owner->vtbl[2];
  pos = ((const ScePspFVector4 *(*)(void *))e->fn)((char *)owner + e->delta);
  dx = self->base.targetPos.x - pos->x;
  dy = self->base.targetPos.y - pos->y;
  dz = self->base.targetPos.z - pos->z;
  dist = dx * dx + dy * dy + dz * dz;
  if (dist < 1.1920929e-07f) {
    return 0;
  }
  return 1;
}
