// bdc 0x088b09c0 ActorStageObjPropCtor
#include "bdc.h"

/* Constructor of the small knock-over props (categories 3 and 7: cars, containers, trees, pipe
   units, boats, buses, signs, billboards, street lights; 0x380 bytes, built by
   `ActorStageObjCreateByKind`): `ActorStageObjBaseCtor`, vtable `0x08af2ae4`, clears `step`
   and `settleFrames`, state `+0x350` (1 except kinds 0x19, 0x2c, 0x3d, 0x73/0x74, 0x88/0x89 =
   trees and similar rooted props, which get 0), radius `+0x320 = 0.5 * box diagonal`, half height
   `+0x324 = 0.5 * (max.y - min.y)`, no physics box, lighting on, fade 1, contact vectors
   `+0x360/+0x370` zeroed (the VFPU bank's C720). Returns `self`. */

ActorStageObjProp *ActorStageObjPropCtor(ActorStageObjProp *self, int kind, float *pos)
{
  float *max;
  float *min;
  float dx;
  float dy;
  float dz;
  float maxY;

  ActorStageObjBaseCtor(&self->base, kind, pos);
  self->base.base.base.vtable = g_actorStageObjPropVtbl;
  self->step = 0;
  self->settleFrames = 0;
  self->state = 1;
  if (kind < 0x73) {
    if (kind == 0x3d || kind == 0x2c || kind == 0x19)
      self->state = 0;
  } else if (kind < 0x88) {
    if (kind < 0x75)
      self->state = 0;
  } else if (kind < 0x8a) {
    self->state = 0;
  }

  max = ActorStageObjGetBounds(&self->base) + 4;
  min = ActorStageObjGetBounds(&self->base);
  dx = max[0] - min[0];
  dy = max[1] - min[1];
  dz = max[2] - min[2];
  self->radius = __builtin_sqrtf(dx * dx + dy * dy + dz * dz) * 0.5f;

  maxY = ActorStageObjGetBounds(&self->base)[5];
  self->halfHeight = (maxY - ActorStageObjGetBounds(&self->base)[1]) * 0.5f;
  self->physicsBox = NULL;
  self->base.base.lighting = 1;
  self->base.fade = 1.0f;
  self->contactPos[0] = 0.0f;
  self->contactPos[1] = 0.0f;
  self->contactPos[2] = 0.0f;
  self->contactPos[3] = 0.0f;
  self->contactVel[0] = 0.0f;
  self->contactVel[1] = 0.0f;
  self->contactVel[2] = 0.0f;
  self->contactVel[3] = 0.0f;
  return self;
}
