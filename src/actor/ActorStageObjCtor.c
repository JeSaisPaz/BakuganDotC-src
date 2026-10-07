// bdc 0x088a8a24 ActorStageObjCtor
#include "bdc.h"

/* Constructor of the 0x340-byte stage object (scenery prop with HP) built by
   `ActorStageObjCreate`. Runs `ActorStageObjBaseCtor``(obj, kind, pos)`, installs its vtable
   `0x08af2864` at `+0x14`, drops the object onto the ground (ground point below the model origin
   via `CollisionFindGroundPoint`, minus the Y offset returned by `ActorStageObjGetBounds`), derives the
   heading `wrap(pi/2 - pos[3])` into `rot[1]`, builds the model matrix (Y rotation times scale,
   translation = `pos` with the ground Y), allocates a 0x680-byte companion from the low
   heap (`BtlTargetPointPropCtor` at the ground point, stored at `+0x320`, NULL when the
   allocation fails) and stores `instanceId` at `+0x21c`. Returns `self`. */

ActorStageObj *ActorStageObjCtor(ActorStageObj *self, int kind, const float *pos, u32 instanceId)
{
  float ground[4];
  float unitPos[4];
  float heading;
  float groundY;
  float c;
  float s;
  float sx;
  float sy;
  float sz;
  float *bounds;
  float *m;
  void *unit;
  void *mem;
  bool fromLow;

  ActorStageObjBaseCtor(&self->base, kind, pos);
  m = self->base.base.data->rootMatrix;
  self->base.base.base.vtable = &g_actorStageObjVtbl;
  CollisionFindGroundPoint(ground, &m[12], 0x3fbf2500);
  groundY = ground[1];
  bounds = ActorStageObjGetBounds(&self->base);
  ground[1] = groundY + -bounds[1];
  heading = 1.5707964f - pos[3];
  if (!(heading <= 3.1415927f)) {
    heading = heading - 6.2831855f;
  } else if (heading <= -3.1415927f) {
    heading = heading + 6.2831855f;
  }
  self->base.base.rot[1] = heading;
  m = self->base.base.data->rootMatrix;
  /* Model matrix = Y rotation by the heading times diag(scale): rows written column-major. */
  sx = self->base.base.scale[0];
  sy = self->base.base.scale[1];
  sz = self->base.base.scale[2];
  c = __builtin_cosf(heading);
  s = __builtin_sinf(heading);
  m[0] = c * sx;
  m[1] = 0.0f;
  m[2] = -s * sx;
  m[3] = 0.0f;
  m[4] = 0.0f;
  m[5] = sy;
  m[6] = 0.0f;
  m[7] = 0.0f;
  m[8] = s * sz;
  m[9] = 0.0f;
  m[10] = c * sz;
  m[11] = 0.0f;
  m[12] = 0.0f;
  m[13] = 0.0f;
  m[14] = 0.0f;
  m[15] = 1.0f;
  self->base.base.pos[1] = ground[1];
  m = self->base.base.data->rootMatrix;
  m[12] = self->base.base.pos[0];
  m[13] = self->base.base.pos[1];
  m[14] = self->base.base.pos[2];
  m[15] = self->base.base.pos[3];
  self->cleared324 = 0;
  self->effectHandle = 0;
  self->cleared32c = 0;
  self->base.base.lighting = 1;
  self->base.fade = 1.0f;
  self->unit = NULL;
  unit = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0x680, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != NULL) {
    unitPos[0] = ground[0];
    unitPos[1] = ground[1];
    unitPos[2] = ground[2];
    unitPos[3] = ground[3];
    BtlTargetPointPropCtor(mem, unitPos);
    unit = mem;
  }
  self->unit = unit;
  self->flag330 = 0;
  self->base.instanceId = instanceId;
  return self;
}
