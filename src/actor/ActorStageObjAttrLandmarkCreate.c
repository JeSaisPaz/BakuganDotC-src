// bdc 0x088a7320 ActorStageObjAttrLandmarkCreate
#include "bdc.h"

/* Creates an attribute landmark: allocates 0x3d0 bytes from the low heap, runs
   `ActorStageObjAttrLandmarkCtor``(obj, kind, pos, arg)`, creates its HP gauge
   (`ActorStageObjEnsureHpGauge`) and spawns the ground ring effect `0x3c + auraType` under it
   (`CollisionFindGroundPoint`, `GfxEffectSpawnWithOwner` on `g_btlUnitEffectMgr`), sized
   (`auraRadius`, 150, `auraRadius`, 0) with alpha `auraRadius * 0.0001`. Returns the landmark.
   Called by `ActorStageObjRecordSpawn`. A failed allocation is not handled (NULL is used). */

void *ActorStageObjAttrLandmarkCreate(s32 kind, float *pos, s32 arg)
{
  bool fromLow;
  ActorStageObjAttrLandmark *mem;
  ActorStageObjAttrLandmark *self;
  GfxEffect *ring;
  float ground[4] __attribute__((aligned(16)));
  float radius;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0x3d0, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self = NULL;
  if (mem != NULL) {
    ActorStageObjAttrLandmarkCtor(mem, kind, pos, arg);
    self = mem;
  }
  ActorStageObjEnsureHpGauge(&self->base);
  CollisionFindGroundPoint(ground, self->base.base.pos, 0x3fbf2500);
  ring = GfxEffectSpawnWithOwner(g_btlUnitEffectMgr, self->auraType + 0x3c, ground, self);
  radius = self->auraRadius;
  ring->size[0] = radius;
  ring->size[1] = 150.0f;
  ring->size[2] = radius;
  ring->size[3] = 0.0f;
  ring->color[3] = self->auraRadius * 0.0001f;
  return self;
}
