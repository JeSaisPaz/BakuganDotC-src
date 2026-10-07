// bdc 0x088f6bf8 GameQuestCamPathModeCheckBehind
#include "bdc.h"

/* Clears `behind`, and sets it when the mode is attached to a segment, its target moved
   (`GameQuestCamModeTargetMoved`) and both the camera direction (`entry->+0x50`) and the target
   offset (followed position minus `targetPos`) point against the segment normal (`seg+0x10`). */

typedef struct BehindOwner {
  void *unk0;
  const VtblEntry *vtbl; /* +0x04 */
} BehindOwner;

typedef struct BehindSegment {
  char pad[0x10];
  float normal[4];
} BehindSegment;

typedef struct BehindEntry {
  char pad[0x50];
  float dir[4];
} BehindEntry;

void GameQuestCamPathModeCheckBehind(GameQuestCamPathMode *self)
{
  BehindOwner *owner;
  const VtblEntry *e;
  const float *pos;
  const BehindSegment *seg;
  const BehindEntry *entry;
  float dx, dy, dz;
  float dir[3];

  seg = (const BehindSegment *)self->segment;
  self->base.behind = 0;
  if (seg == (const BehindSegment *)0 || GameQuestCamModeTargetMoved(&self->base) == 0) {
    return;
  }
  owner = (BehindOwner *)self->base.base.base.followed;
  e = &owner->vtbl[2];
  pos = ((const float *(*)(void *))e->fn)((char *)owner + e->delta);
  dx = pos[0] - self->base.targetPos.x;
  dy = pos[1] - self->base.targetPos.y;
  dz = pos[2] - self->base.targetPos.z;
  entry = (const BehindEntry *)*(void **)self->base.entry;
  dir[0] = entry->dir[0];
  dir[1] = entry->dir[1];
  dir[2] = entry->dir[2];
  seg = (const BehindSegment *)self->segment;
  if (!(dir[0] * seg->normal[0] + dir[1] * seg->normal[1] + dir[2] * seg->normal[2] < 0.0f)) {
    return;
  }
  seg = (const BehindSegment *)self->segment;
  if (dx * seg->normal[0] + dy * seg->normal[1] + dz * seg->normal[2] < 0.0f) {
    self->base.behind = 1;
  }
}
