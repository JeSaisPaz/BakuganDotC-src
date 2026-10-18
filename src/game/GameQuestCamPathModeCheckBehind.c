// bdc 0x088f6bf8 GameQuestCamPathModeCheckBehind
#include "bdc.h"

/* Clears `behind`, and sets it when the mode is attached to a segment, its target moved
   (`GameQuestCamModeTargetMoved`) and both the camera direction (`entry->+0x50`) and the target
   offset (followed position minus `targetPos`) point against the segment normal (`seg+0x10`). */

void GameQuestCamPathModeCheckBehind(GameQuestCamPathMode *self)
{
  GameQuestPathCursor *owner;
  const VtblEntry *e;
  const float *pos;
  const GameQuestPathSegment *seg;
  const GameQuestCamSpring *look;
  float dx, dy, dz;
  float dir[3];

  seg = (const GameQuestPathSegment *)self->segment;
  self->base.behind = 0;
  if (seg == (const GameQuestPathSegment *)0 || GameQuestCamModeTargetMoved(&self->base) == 0) {
    return;
  }
  owner = (GameQuestPathCursor *)self->base.base.base.followed;
  e = &owner->vtbl[2];
  pos = ((const float *(*)(void *))e->fn)((char *)owner + e->delta);
  dx = VfSub(pos[0], self->base.targetPos.x);
  dy = VfSub(pos[1], self->base.targetPos.y);
  dz = VfSub(pos[2], self->base.targetPos.z);
  look = *(GameQuestCamSpring **)self->base.entry;
  dir[0] = look->step.x;
  dir[1] = look->step.y;
  dir[2] = look->step.z;
  seg = (const GameQuestPathSegment *)self->segment;
  if (!(VfDot3(dir[0], dir[1], dir[2], seg->dir.x, seg->dir.y, seg->dir.z) < 0.0f)) {
    return;
  }
  seg = (const GameQuestPathSegment *)self->segment;
  if (VfDot3(dx, dy, dz, seg->dir.x, seg->dir.y, seg->dir.z) < 0.0f) {
    self->base.behind = 1;
  }
}
