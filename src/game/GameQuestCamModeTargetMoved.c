// bdc 0x088fd524 GameQuestCamModeTargetMoved
#include "bdc.h"

/* Returns 0 when the squared xyz distance between the followed object's position (slot `+0x14`
   of `*(cam+8)`) and the captured `targetPos` (`GameQuestCamModeCaptureTargetPos`) is below
   FLT_EPSILON, else 1 (also 1 for a NaN distance). */

typedef struct CamMovedOwnerVtbl {
  char pad[0x10];
  VtblEntry getPos;
} CamMovedOwnerVtbl;

typedef struct CamMovedOwner {
  void *unk0;
  CamMovedOwnerVtbl *vtbl;
} CamMovedOwner;

int GameQuestCamModeTargetMoved(GameQuestCamModeBase *self)
{
  CamMovedOwner *owner;
  const VtblEntry *e;
  const float *pos;
  float dx;
  float dy;
  float dz;
  float dist;

  owner = (CamMovedOwner *)self->base.base.followed;
  e = &owner->vtbl->getPos;
  pos = ((const float *(*)(void *))e->fn)((char *)owner + e->delta);
  dx = self->targetPos.x - pos[0];
  dy = self->targetPos.y - pos[1];
  dz = self->targetPos.z - pos[2];
  dist = dx * dx + dy * dy + dz * dz;
  if (dist < 1.1920929e-07f) {
    return 0;
  }
  return 1;
}
