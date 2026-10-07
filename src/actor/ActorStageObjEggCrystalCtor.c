// bdc 0x088a3ad0 ActorStageObjEggCrystalCtor
#include "bdc.h"

/* Constructor of the egg crystal stage object (kind 0xb0 `EGG_CRYSTAL`, `fz_crystal02_fbx.gmo`,
   0x390 bytes, built by `ActorStageObjCreateByKind`): `ActorStageObjBaseCtor``(obj, 0xb0,
   pos)`, vtable `g_actorStageObjEggCrystalVtbl`, snaps `pos` to the ground
   (`CollisionFindGroundPoint`) minus the model's Y offset (`ActorStageObjGetBounds`),
   lighting off (`+0xbc = 0`), alpha `+0x228 = 0`, builds the 0x680-byte companion unit
   `BtlTargetPointCtor(150, 70, …, 1, 5)` 70 units above the ground point at `unit` (`+0x320`,
   NULL when the allocation fails), zeroes `debrisVel` and builds the Y-rotation matrix of the
   heading `pos[3]` in the model's root matrix. Returns `self`. */

ActorStageObjEggCrystal *ActorStageObjEggCrystalCtor(ActorStageObjEggCrystal *self, float *pos)

{
  float ground[4] __attribute__((aligned(16)));
  float unitPos[4] __attribute__((aligned(16)));
  void *unit;
  void *mem;
  bool fromLow;
  float *bounds;
  float y;
  float heading;
  float c;
  float s;
  int i;
  GmoModel *data;

  ActorStageObjBaseCtor(&self->base, 0xb0, pos);
  self->base.base.base.vtable = g_actorStageObjEggCrystalVtbl;
  CollisionFindGroundPoint(ground, &self->base.base.data->rootMatrix[12], 0x3fbf2500);
  for (i = 0; i < 4; i++) {
    pos[i] = ground[i];
  }
  self->step = 0;
  self->stepData = 0;
  self->state = 0;
  self->element = 0;
  self->aiLevel = 5;
  self->base.base.lighting = 0;
  self->base.fade = 0.0f;
  self->unit = NULL;
  ground[1] = ground[1] + 70.0f;
  unit = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0x680, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != NULL) {
    for (i = 0; i < 4; i++) {
      unitPos[i] = ground[i];
    }
    BtlTargetPointCtor(150.0f, 70.0f, (BtlBakugan *)mem, unitPos, true, 5);
    unit = mem;
  }
  self->unit = unit;
  self->broken = 0;
  self->base.record = NULL;
  self->hatchKind = 0;
  self->ownerSlot = 0;
  self->ownerIndex = 0;
  for (i = 0; i < 4; i++) {
    self->debrisVel[i] = 0.0f;
  }
  self->owner = NULL;
  y = pos[1];
  bounds = ActorStageObjGetBounds(&self->base);
  y = y + -(bounds[1] * self->base.base.scale[1]);
  pos[1] = y;
  self->base.base.pos[1] = y;
  heading = pos[3];
  self->base.base.rot[1] = heading;
  data = self->base.base.data;
  c = __builtin_cosf(heading);
  s = __builtin_sinf(heading);
  data->rootMatrix[0] = c;
  data->rootMatrix[1] = 0.0f;
  data->rootMatrix[2] = -s;
  data->rootMatrix[3] = 0.0f;
  data->rootMatrix[4] = 0.0f;
  data->rootMatrix[5] = 1.0f;
  data->rootMatrix[6] = 0.0f;
  data->rootMatrix[7] = 0.0f;
  data->rootMatrix[8] = s;
  data->rootMatrix[9] = 0.0f;
  data->rootMatrix[10] = c;
  data->rootMatrix[11] = 0.0f;
  data->rootMatrix[12] = 0.0f;
  data->rootMatrix[13] = 0.0f;
  data->rootMatrix[14] = 0.0f;
  data->rootMatrix[15] = 1.0f;
  data = self->base.base.data;
  for (i = 0; i < 4; i++) {
    data->rootMatrix[12 + i] = self->base.base.pos[i];
  }
  return self;
}
