// bdc 0x088a5b64 ActorStageObjMineCtor
#include "bdc.h"

/* Constructor of the mine stage objects (category 0xc: kind 0xb3 `GIMMICK_01_KIRAI` floating mine,
   0xb4 `GIMMICK_02_JIRAI` land mine; 0x340 bytes): `ActorStageObjBaseCtor`, vtable
   g_actorStageObjMineVtbl, scale 3 (1 for 0xb4), trigger radius = 0.4 * bounds diagonal * scale,
   lifts the model root by the bounds' min Y (`ActorStageObjGetBounds`) and copies it to pos.y,
   `ActorStageObjRebuildMatrix(obj, 1)`, then clears spare320/fuseTimer/state and the three flag
   bytes, turns lighting on and sets fade 1. Returns `self`. */

ActorStageObjMine *ActorStageObjMineCtor(ActorStageObjMine *self, int kind, float *pos)
{
  float *maxCorner;
  float *minCorner;
  float *rootY;
  float oldY;
  float *bounds;
  float dx;
  float dy;
  float dz;
  float size;

  ActorStageObjBaseCtor(&self->base, kind, pos);
  self->base.base.base.vtable = g_actorStageObjMineVtbl;
  maxCorner = ActorStageObjGetBounds(&self->base) + 4;
  minCorner = ActorStageObjGetBounds(&self->base);
  if (kind == 0xb4) {
    self->base.base.scale[0] = 1.0f;
    self->base.base.scale[1] = 1.0f;
    self->base.base.scale[2] = 1.0f;
    self->base.base.scale[3] = 0.0f;
  } else {
    self->base.base.scale[0] = 3.0f;
    self->base.base.scale[1] = 3.0f;
    self->base.base.scale[2] = 3.0f;
    self->base.base.scale[3] = 0.0f;
  }
  /* |max - min| over xyz (vsub.q / vdot.t / vsqrt.s) */
  dx = maxCorner[0] - minCorner[0];
  dy = maxCorner[1] - minCorner[1];
  dz = maxCorner[2] - minCorner[2];
  size = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
  self->base.triggerRadius = size * 0.4f * self->base.base.scale[0];

  rootY = &self->base.base.data->rootMatrix[13];
  oldY = *rootY;
  bounds = ActorStageObjGetBounds(&self->base);
  *rootY = oldY + bounds[1];
  self->base.base.pos[1] = self->base.base.data->rootMatrix[13];
  ActorStageObjRebuildMatrix(&self->base, 1);

  self->spare320 = 0;
  self->fuseTimer = 0;
  self->state = 0;
  self->base.base.lighting = 1;
  self->base.fade = 1.0f;
  self->recordMarked = 0;
  self->initialized = 0;
  self->fuseEffect = 0;
  return self;
}
