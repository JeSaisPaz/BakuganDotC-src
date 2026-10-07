// bdc 0x088a45b0 ActorStageObjEggCrystalUpdate
#include "bdc.h"

/* Update method of the egg crystal (vtable `0x08af2524` slot 7): does nothing once `broken`
   (`+0x384`) is set. Calls the break virtual (`+0x5c`) and returns as soon as its owner is gone
   (`ActorStageObjEggCrystalOwnerAlive`); otherwise runs the distance fade
   (`ActorStageObjUpdateFade`, radius `+0x1f0 * 5`, range 4000), sets the draw alpha, hides
   itself while a battle demo runs with the camera task alive. When destroyed (`+0x281`) it sets
   `broken`/`removeRequest` and calls the break virtual; else it reacts to hits on the companion
   unit's collider (`CollisionColliderTickHit`, `ActorStageObjEggCrystalOnHit`), shakes for 16
   frames (`ActorStageObjGetShakeOffset`), rebuilds the matrix
   (`ActorStageObjEggCrystalUpdateTransform`) and runs state `+0x380` from
   `g_actorStageObjEggCrystalStateFns`: `ActorStageObjEggCrystalState00Idle`,
   `ActorStageObjEggCrystalState01Appear`, `ActorStageObjEggCrystalState02Hatch`. */

void ActorStageObjEggCrystalUpdate(ActorStageObjEggCrystal *self)

{
  const VtblEntry *brk;
  const MemberFnPtr *member;
  u8 *obj;
  void *fn;
  s32 state;
  float restX;
  float restZ;
  u8 phase;

  if (self->broken != 0) {
    return;
  }
  if (ActorStageObjEggCrystalOwnerAlive(self) == 0) {
    brk = &((const VtblEntry *)self->base.base.base.vtable)[11];
    ((void (*)(void *))brk->fn)((u8 *)self + brk->delta);
    return;
  }
  self->base.visible =
      ActorStageObjUpdateFade(self->base.diagonal * 5.0f, 4000.0f, self, &self->base,
                              &self->base.baseAlpha, (char *)&self->base.fadeState, 0) == 0;
  self->base.base.ambient[3] = self->base.baseAlpha * self->base.fade;
  if (BtlCameraTaskExists() != 0) {
    BtlGetCameraTask();
    if (BtlIsDemoRunning()) {
      self->base.base.visible = 0;
    }
  }
  if (self->base.dead != 0) {
    if (self->broken == 0) {
      self->broken = 1;
      self->base.removeRequest = 1;
      brk = &((const VtblEntry *)self->base.base.base.vtable)[11];
      ((void (*)(void *))brk->fn)((u8 *)self + brk->delta);
    }
    return;
  }
  if (self->unit != NULL &&
      CollisionColliderTickHit((CoreNode *)((BtlTargetPoint *)self->unit)->base.collider0)) {
    ActorStageObjEggCrystalOnHit(self);
  }
  if (self->base.shaking != 0) {
    restX = self->base.shakeX;
    self->base.base.pos[0] =
        restX + (float)ActorStageObjGetShakeOffset(&self->base, self->base.shakePhase & 0x1f);
    restZ = self->base.shakeZ;
    self->base.base.pos[2] =
        restZ + (float)ActorStageObjGetShakeOffset(&self->base, (self->base.shakePhase + 8) & 0x1f);
    self->base.shakePhase = self->base.shakePhase + 1;
    phase = self->base.shakePhase;
    if (phase >= 16) {
      self->base.base.pos[0] = self->base.shakeX;
      self->base.base.pos[2] = self->base.shakeZ;
      self->base.shaking = 0;
    }
  }
  ActorStageObjEggCrystalUpdateTransform(self);
  state = self->state;
  if (state >= 0 && (u32)state < 3) {
    member = &g_actorStageObjEggCrystalStateFns[state];
    obj = (u8 *)self + member->delta;
    fn = member->pfn;
    if (member->index != 0) {
      const VtblEntry *vtbl = *(const VtblEntry **)(obj + (intptr_t)member->pfn);
      const VtblEntry *entry = &vtbl[member->index];

      fn = entry->fn;
      obj += entry->delta;
    }
    ((void (*)(void *))fn)(obj);
  }
}
