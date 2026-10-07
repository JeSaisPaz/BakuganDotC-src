// bdc 0x088f8308 GameQuestCamPointModeCheckBehind
#include "bdc.h"

/* When the target moved, clears `behind` and sets it again if the target's offset from
   `targetPos` points against the view direction `viewDir` (dot product below zero). */

typedef struct CamBehindOwnerVtbl {
  char pad[0x10];
  VtblEntry getPos;
} CamBehindOwnerVtbl;

typedef struct CamBehindOwner {
  void *unk0;
  CamBehindOwnerVtbl *vtbl;
} CamBehindOwner;

void GameQuestCamPointModeCheckBehind(GameQuestCamPointMode *self)
{
  CamBehindOwner *owner;
  const VtblEntry *e;
  const float *pos;
  float dx, dy, dz;

  if (GameQuestCamPointModeTargetMoved(self) == 0) {
    return;
  }
  self->base.behind = 0;
  owner = (CamBehindOwner *)self->base.base.base.followed;
  e = &owner->vtbl->getPos;
  pos = ((const float *(*)(void *))e->fn)((char *)owner + e->delta);
  dx = pos[0] - self->base.targetPos.x;
  dy = pos[1] - self->base.targetPos.y;
  dz = pos[2] - self->base.targetPos.z;
  /* The listing also copies the 16 bytes at (*entry)+0x50 to an unused stack slot (dead). */
  if (dx * self->viewDir.x + dy * self->viewDir.y + dz * self->viewDir.z < 0.0f) {
    self->base.behind = 1;
  }
}
