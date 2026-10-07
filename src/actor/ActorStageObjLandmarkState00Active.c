// bdc 0x088a2e84 ActorStageObjLandmarkState00Active
#include "bdc.h"

/* State 0 of the landmark (`ActorStageObjLandmarkUpdate`); does nothing once `removed` is set.
   Updates the occlusion fade (`ActorStageObjLandmarkUpdateFade`) and sets the draw alpha
   (`ambient[3] = baseAlpha * fade`). A dead landmark sets `removed` and `removeRequest` and calls
   its virtual Break (vtable entry 11, `ActorStageObjLandmarkBreak`), then returns. Otherwise it
   reacts to a hit on the companion unit's collider (`CollisionColliderTickHit`) with
   `ActorStageObjLandmarkOnHit` while no helper collider exists, shakes the model for 16 frames
   after a hit (X/Z offsets from `ActorStageObjGetShakeOffset` around `shakeX`/`shakeZ`), spins
   the orb (`orbAngle` += 0.03 rad, wrapped into (-pi, pi]), writes the Y rotation by that angle
   into the orb node's localMatrix rows 0..2, and rebuilds the model matrix
   (`ActorStageObjUpdateTransform`, `ActorStageObjStepUvScrolls`). */

void ActorStageObjLandmarkState00Active(ActorStageObjLandmark *self)
{
  const VtblEntry *ent;
  bool hit;
  float shakeX;
  float shakeZ;
  s32 offset;
  u8 phase;
  float c;
  float s;
  float *m;

  if (self->removed != 0) {
    return;
  }
  hit = false;
  ActorStageObjLandmarkUpdateFade(self);
  self->base.base.ambient[3] = self->base.baseAlpha * self->base.fade;
  if (self->base.dead != 0) {
    if (self->removed == 0) {
      ent = &((const VtblEntry *)self->base.base.base.vtable)[11];
      self->removed = 1;
      self->base.removeRequest = 1;
      ((void (*)(void *))ent->fn)((char *)self + ent->delta);
    }
    return;
  }
  if (self->unit != NULL && CollisionColliderTickHit(&self->unit->base.collider0->node)) {
    hit = true;
  }
  if (self->helper == NULL && hit) {
    ActorStageObjLandmarkOnHit(self);
  }
  if (self->base.shaking != 0) {
    shakeX = self->base.shakeX;
    offset = ActorStageObjGetShakeOffset(&self->base, self->base.shakePhase & 0x1f);
    phase = self->base.shakePhase;
    shakeZ = self->base.shakeZ;
    self->base.base.pos[0] = shakeX + (float)offset;
    offset = ActorStageObjGetShakeOffset(&self->base, (phase + 8) & 0x1f);
    self->base.shakePhase = self->base.shakePhase + 1;
    self->base.base.pos[2] = shakeZ + (float)offset;
    if (self->base.shakePhase >= 0x10) {
      self->base.base.pos[0] = self->base.shakeX;
      self->base.base.pos[2] = self->base.shakeZ;
      self->base.shaking = 0;
    }
  }
  self->orbAngle = self->orbAngle + 0.0299999993f;
  if (!(self->orbAngle <= 3.14159274f)) {
    self->orbAngle = self->orbAngle - 6.28318548f;
  } else if (self->orbAngle <= -3.14159274f) {
    self->orbAngle = self->orbAngle + 6.28318548f;
  }
  /* The binary first multiplies an uninitialised stack matrix by Rx and Rz and then overwrites it
     with the plain Y rotation (see Notes): only Ry survives. vrot with angle * S703 (2/pi) in
     quarter turns is cos/sin of the angle in radians. */
  c = __builtin_cosf(self->orbAngle);
  s = __builtin_sinf(self->orbAngle);
  if (self->orbNode != NULL) {
    m = self->orbNode->localMatrix;
    m[0] = c;
    m[1] = 0.0f;
    m[2] = -s;
    m[3] = 0.0f;
    m[4] = 0.0f;
    m[5] = 1.0f;
    m[6] = 0.0f;
    m[7] = 0.0f;
    m[8] = s;
    m[9] = 0.0f;
    m[10] = c;
    m[11] = 0.0f;
  }
  ActorStageObjUpdateTransform(&self->base);
  ActorStageObjStepUvScrolls(&self->base);
}
