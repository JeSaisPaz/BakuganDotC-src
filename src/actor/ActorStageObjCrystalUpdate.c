// bdc 0x088b4868 ActorStageObjCrystalUpdate
#include "bdc.h"

/* Update method (vtable `0x08af2b94` slot 7) of the crystal stage object
   (`ActorStageObjCrystalCtor`): while the field task exists (`GameFieldTaskExists`) and
   `+0x398` is clear it zeroes the knock direction `+0x370` (bank constant C720) and calls the
   break virtual (`+0x5c`,
   `ActorStageObjCrystalBreak`); steps the UV scroll (`ActorStageObjCrystalStepUvScroll`). While
   alive (`+0x394` clear) it runs the distance fade (`ActorStageObjUpdateFade`, radius `+0x1f0 *
   5`, range 4000; culled flag `+0x284`), sets the draw alpha `+0x6c = +0x238 * +0x228`, and —
   unless destroyed (`+0x281`) — reacts to hits on the companion's collider
   (`CollisionColliderTickHit`, `ActorStageObjCrystalOnHit`), shakes for 16 frames
   (`ActorStageObjGetShakeOffset`, rest position `+0x2b4`/`+0x2b8`), rebuilds the matrix
   (`ActorStageObjCrystalUpdateTransform`) and runs state `+0x390` from `g_actorStageObjCrystalStateFns`
   (`ActorStageObjCrystalState00Idle`, `ActorStageObjCrystalState01Shoot`). When destroyed it
   sets `+0x394`/`+0x282`, a 500-frame countdown `+0x338` and calls the break virtual; afterwards it
   counts down and queues itself for deletion (`CoreObjectDeferDelete`). */

void ActorStageObjCrystalUpdate(ActorStageObjCrystal *self)

{
  const VtblEntry *brk;
  const MemberFnPtr *member;
  u8 *obj;
  void *fn;
  s32 timer;
  s32 state;
  float restX;
  float restZ;
  u8 phase;

  if (GameFieldTaskExists() != 0 && self->fieldEnabled == 0) {
    /* sv.q of the bank constant C720 = (0, 0, 0, 0) */
    self->knockDir[0] = 0.0f;
    self->knockDir[1] = 0.0f;
    self->knockDir[2] = 0.0f;
    self->knockDir[3] = 0.0f;
    brk = &((const VtblEntry *)self->base.base.base.vtable)[11];
    ((void (*)(void *))brk->fn)((u8 *)self + brk->delta);
  }
  ActorStageObjCrystalStepUvScroll(self);
  if (self->destroyed != 0) {
    timer = self->removeTimer;
    self->removeTimer = timer - 1;
    if (timer < 1) {
      CoreObjectDeferDelete((CoreObject *)self, 0);
    }
    return;
  }
  self->base.visible =
      ActorStageObjUpdateFade(self->base.diagonal * 5.0f, 4000.0f, self, &self->base,
                              &self->base.baseAlpha, (char *)&self->base.fadeState, 0) == 0;
  self->base.base.ambient[3] = self->base.baseAlpha * self->base.fade;
  if (self->base.dead != 0) {
    if (self->destroyed == 0) {
      self->destroyed = 1;
      self->base.removeRequest = 1;
      self->removeTimer = 500;
      brk = &((const VtblEntry *)self->base.base.base.vtable)[11];
      ((void (*)(void *))brk->fn)((u8 *)self + brk->delta);
    }
    return;
  }
  if (self->unit != NULL &&
      CollisionColliderTickHit((CoreNode *)((BtlTargetPoint *)self->unit)->base.collider0)) {
    ActorStageObjCrystalOnHit(self);
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
  ActorStageObjCrystalUpdateTransform(self);
  state = self->state;
  if (state >= 0 && (u32)state < 2) {
    member = &g_actorStageObjCrystalStateFns[state];
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
