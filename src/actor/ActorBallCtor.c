// bdc 0x088b8038 ActorBallCtor
#include "bdc.h"

/* Constructor of the thrown/ball-form Bakugan model (`ActorBall`, 0x200 bytes, `GfxModelCtor`
   subclass, vtable `g_actorBallVtable`: slot 1 `ActorBallDtor`, 7 `ActorBallUpdate`, 8
   `ActorBallDraw`; created by `ActorBallCreate`): builds the GMO model for `kind`
   (`g_btlModelNames``[kind]`), installs the vtable, stores `kind` at `+0x08` and the next id
   from `g_actorBallNextId` at `+0x0c`, sets `serialTag` to (`g_actorBallSerial``++ % 0xdf) +
   0x60`, enables lighting, clears fog and packs `g_colorBlack` as the fog colour (each lane
   saturated to [0, 1], scaled by 255, truncated, packed to bytes, `x` in the low byte), sets specular 0.6/power 10,
   loads the kind's ball motions (`ActorBallLoadMotions`), starts motion slot 1 at speed 0.2
   (`ActorBallPlayMotion`), appends itself to the object list `g_actorBallList` (set by
   `ActorBallSetList`), zeroes its six
   vectors, clears its state bytes, runs `ActorBallModelMaterialCallback` on every material and
   returns `ball`. */

CoreObject *ActorBallCtor(CoreObject *ball, u32 kind)
{
  ActorBall *self = (ActorBall *)ball;
  float specular[4] __attribute__((aligned(16)));
  s32 n;
  u32 packed;

  GfxModelCtor(&self->base, g_btlModelNames[kind], 0);
  self->base.base.vtable = &g_actorBallVtable;
  self->base.base.unk08 = kind;
  n = g_actorBallNextId;
  g_actorBallNextId = n + 1;
  self->base.base.id = n;
  self->state = 0;
  self->owner = NULL;
  n = g_actorBallSerial;
  g_actorBallSerial = n + 1;
  self->serialTag = n % 0xdf + 0x60;
  self->motionMap = NULL;
  self->base.fogColor = 0;
  self->base.fogFar = 0.0f;
  self->base.fogNear = 0.0f;
  self->base.lighting = 1;
  /* vsat0.q, vscl.q by bank S701 (255), vf2iz.q 23, vi2uc.q */
  packed = (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.x) * 255.0f, 23)) |
           (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.y) * 255.0f, 23)) << 8 |
           (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.z) * 255.0f, 23)) << 16 |
           (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.w) * 255.0f, 23)) << 24;
  self->base.fogColor = packed;
  specular[0] = 0.6f;
  specular[1] = 0.6f;
  specular[2] = 0.6f;
  specular[3] = 1.0f;
  GfxModelSetSpecular(10.0f, &self->base, specular, NULL);
  self->motionFile = NULL;
  ActorBallLoadMotions(ball);
  ActorBallPlayMotion(0.2f, ball, 1, 0, false);
  self->unk148 = 0;
  self->flag1d1 = 0;
  CoreObjectListAppend(ball, (CoreObjectList *)g_actorBallList);
  self->unk190 = 0;
  /* sv.q of bank C720 (zero vector) */
  self->hitNormal[0] = 0.0f;
  self->hitNormal[1] = 0.0f;
  self->hitNormal[2] = 0.0f;
  self->hitNormal[3] = 0.0f;
  self->hitPoint[0] = 0.0f;
  self->hitPoint[1] = 0.0f;
  self->hitPoint[2] = 0.0f;
  self->hitPoint[3] = 0.0f;
  self->vec160[0] = 0.0f;
  self->vec160[1] = 0.0f;
  self->vec160[2] = 0.0f;
  self->vec160[3] = 0.0f;
  self->flag1d0 = 0;
  self->vec1e0[0] = 0.0f;
  self->vec1e0[1] = 0.0f;
  self->vec1e0[2] = 0.0f;
  self->vec1e0[3] = 0.0f;
  self->hitType = 0;
  self->travelled = 0.0f;
  self->noGroundProbe = 0;
  self->groundPoint[0] = 0.0f;
  self->groundPoint[1] = 0.0f;
  self->groundPoint[2] = 0.0f;
  self->groundPoint[3] = 0.0f;
  self->groundNormal[0] = 0.0f;
  self->groundNormal[1] = 0.0f;
  self->groundNormal[2] = 0.0f;
  self->groundNormal[3] = 0.0f;
  GfxModelForEachMaterial(&self->base, ActorBallModelMaterialCallback, NULL);
  self->inFlight = 0;
  self->flag1fc = 0;
  return ball;
}
