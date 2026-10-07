// bdc 0x088ad794 ActorStageObjState00Update
#include "bdc.h"

/* State 0 handler of the shared stage-object state machine (state `+0x304`, `MemberFnPtr` table
   `0x08a842f8`, run by `ActorStageObjUpdate`): the active/hit-reaction state. Does nothing
   without a collider. Clears `hitBy`, then runs the sub-step `step`: step 0 clears
   `velocity` (bank-zero row C720), clears collider flag bit 1 and moves to step 1; step 2 (death)
   sets `dead`, `fade` = 1, resets `step` to 0 and picks the next state from `category` (playing
   the kind's break sound and switching the materials to translucent for categories 0/1/3/5; an
   unknown category sets `removeRequest`). Then it ticks the collider hit
   (`CollisionColliderTickHit`). Without a hit it advances the 16-frame shake of the root node
   around `restPos`. On a hit: category 8 only re-arms the collider and returns; otherwise it
   records the attacker and hit position, returns early (re-arming with cooldown 1) when a
   landmark is hit by the player, spawns the hit sparks (world effect 9 pulled 4 units towards the
   camera, plus unit effect 0 for categories 1, 2, 5, 6), plays the hit sound (`0x2000cb` /
   `0x2000cf` vary by `g_actorStageObjFrame % 4`), applies `BtlCalcDamage` via
   `ActorStageObjApplyDamage`, and either moves to step 2 (HP <= 0, collider layer 10) or starts
   a new shake. Step 0 reads the bank zero vector C720 = (0, 0, 0, 0). */

void ActorStageObjState00Update(ActorStageObjBase *self)
{
  float effPos[4] __attribute__((aligned(16)));
  float camOff[4];
  CollisionCollider *col;
  const VtblEntry *vt;
  GmoNode *node;
  s32 step;
  s32 cat;
  s32 kind;
  s32 hitClass;
  s32 attackId;
  s32 soundId;
  float rest;
  s32 offset;
  int i;

  if (self->collider == NULL) {
    return;
  }
  step = self->step;
  self->hitBy = NULL;
  if (step < 1) {
    if (step >= 0) {
      /* pos is reloaded and stored back unchanged; velocity = bank C720 (zero vector) */
      for (i = 0; i < 4; i++) {
        self->base.velocity[i] = 0.0f;
      }
      col = (CollisionCollider *)self->collider;
      col->flags = col->flags & ~2u;
      self->step = 1;
    }
  } else if (step >= 2 && step < 3) {
    self->shakePhase = 0;
    self->fade = 1.0f;
    self->dead = 1;
    self->step = 0;
    switch (self->category) {
    case 0:
      kind = self->kind;
      if (kind == 2) {
        if (SndHasListener()) {
          SndEmitterCreateAtPos(SndGetListener(), 0x2000e3, self->base.pos, 0, 1);
        }
        self->state = 5;
        break;
      }
      if (kind == 6) {
        if (SndHasListener()) {
          SndEmitterCreateAtPos(SndGetListener(), ((const s32 *)self->soundParams)[1],
                                self->base.pos, 0, 1);
        }
        self->state = 6;
        break;
      }
      /* fall through: other kinds behave like categories 1/3/5 */
    case 1:
    case 3:
    case 5:
      if (SndHasListener()) {
        SndEmitterCreateAtPos(SndGetListener(), ((const s32 *)self->soundParams)[1],
                              self->base.pos, 0, 1);
      }
      self->base.lighting = 1;
      GfxModelForEachMaterial(&self->base, ActorStageObjMaterialSetTranslucent, NULL);
      self->state = 4;
      break;
    case 2:
      if (SndHasListener()) {
        SndEmitterCreateAtPos(SndGetListener(), ((const s32 *)self->soundParams)[1],
                              self->base.pos, 0, 1);
      }
      self->state = 2;
      break;
    case 4:
      self->state = 7;
      break;
    case 6:
      self->state = 9;
      break;
    case 7:
      self->state = 8;
      break;
    default:
      self->removeRequest = 1;
      break;
    }
  }

  if (!CollisionColliderTickHit(&((CollisionCollider *)self->collider)->node)) {
    if (self->shaking == 0) {
      return;
    }
    if (self->rootNode != NULL) {
      rest = self->restPos[0];
      offset = ActorStageObjGetShakeOffset(self, self->shakePhase & 0x1f);
      ((GmoNode *)self->rootNode)->localMatrix[12] = rest + (float)offset;
      rest = self->restPos[2];
      offset = ActorStageObjGetShakeOffset(self, (self->shakePhase + 8) & 0x1f);
      ((GmoNode *)self->rootNode)->localMatrix[14] = rest + (float)offset;
    }
    self->shakePhase = self->shakePhase + 1;
    if (self->shakePhase < 0x10) {
      return;
    }
    if (self->rootNode != NULL) {
      node = (GmoNode *)self->rootNode;
      for (i = 0; i < 4; i++) {
        node->localMatrix[12 + i] = self->restPos[i];
      }
    }
    self->matrixDirty = 1;
    self->shaking = 0;
    return;
  }

  if (self->category == 8) {
    col = (CollisionCollider *)self->collider;
    col->hitTimer = 1;
    col->flags = col->flags | 1;
    ((CollisionCollider *)self->collider)->cooldown = 0;
    return;
  }
  col = (CollisionCollider *)self->collider;
  self->hitBy = col->hitAttacker;
  self->hitPos[0] = col->hitPos.x;
  self->hitPos[1] = col->hitPos.y;
  self->hitPos[2] = col->hitPos.z;
  self->hitPos[3] = col->hitPos.w;
  if (self->hitBy != NULL && ((BtlBakugan *)self->hitBy)->state == 7) {
    ((BtlBakugan *)self->hitBy)->attackResult = 3;
  }
  if (ActorStageObjIsLandmark(self) != 0 || ActorStageObjIsAttrLandmark(self) != 0) {
    if (self->hitBy != NULL && ((BtlBakugan *)self->hitBy)->isPlayer != 0) {
      col = (CollisionCollider *)self->collider;
      col->hitTimer = 1;
      col->flags = col->flags | 1;
      ((CollisionCollider *)self->collider)->cooldown = 1;
      return;
    }
  }

  if (((CollisionCollider *)self->collider)->hitType != 0) {
    /* effPos = hitPos - camera dir * 4 (xyz; w kept from hitPos) */
    col = (CollisionCollider *)self->collider;
    effPos[0] = col->hitPos.x;
    effPos[1] = col->hitPos.y;
    effPos[2] = col->hitPos.z;
    effPos[3] = col->hitPos.w;
    for (i = 0; i < 3; i++) {
      camOff[i] = g_gfxActiveCamera->dir[i] * 4.0f;
    }
    camOff[3] = 0.0f;
    for (i = 0; i < 3; i++) {
      effPos[i] = effPos[i] - camOff[i];
    }
    GfxEffectSpawn(g_worldEffectMgr, 9, effPos);
    cat = self->category;
    if ((cat >= 1 && cat < 3) || (cat >= 5 && cat < 7)) {
      GfxEffectSpawnWithOwner(g_btlUnitEffectMgr, 0, effPos, self);
    }
  }
  self->flags1d0 = 0;
  /* (the asm then copies pos into the dead effPos stack slot) */
  col = (CollisionCollider *)self->collider;
  col->flags = col->flags | 2;
  col = (CollisionCollider *)self->collider;
  col->flags = col->flags & ~2u;
  col = (CollisionCollider *)self->collider;
  hitClass = col->hitParam164;
  attackId = col->hitKind;
  soundId = ((const s32 *)self->soundParams)[0];
  if (soundId == 0x2000cb || soundId == 0x2000cf) {
    soundId = soundId + g_actorStageObjFrame % 4;
  }
  if (SndHasListener()) {
    SndEmitterCreateAtPos(SndGetListener(), soundId, self->base.pos, 0, 1);
  }

  vt = &((const VtblEntry *)self->base.base.vtable)[12];
  if (((int (*)(void *))vt->fn)((u8 *)self + vt->delta) != 0) {
    ActorStageObjAttrLandmarkSetIdle((ActorStageObjAttrLandmark *)self);
    ActorStageObjApplyDamage(
        BtlCalcDamage(self->hitBy, hitClass, attackId, 2,
                      ((ActorStageObjAttrLandmark *)self)->auraType),
        self);
  } else {
    vt = &((const VtblEntry *)self->base.base.vtable)[13];
    if (((int (*)(void *))vt->fn)((u8 *)self + vt->delta) != 0) {
      ActorStageObjApplyDamage(BtlCalcDamage(self->hitBy, hitClass, attackId, 5, 6), self);
    } else {
      ActorStageObjApplyDamage(BtlCalcDamage(self->hitBy, hitClass, attackId, 4, 6), self);
    }
  }

  if (self->shaking != 0) {
    self->base.pos[0] = self->shakeX;
    self->base.pos[2] = self->shakeZ;
    if (self->rootNode != NULL) {
      node = (GmoNode *)self->rootNode;
      for (i = 0; i < 4; i++) {
        node->localMatrix[12 + i] = self->restPos[i];
      }
    }
    self->matrixDirty = 1;
    self->shaking = 0;
    self->shakePhase = 0;
  }
  if ((float)self->hp <= 0.0f) {
    ((CollisionCollider *)self->collider)->layer = 10;
    self->step = 2;
    return;
  }
  vt = &((const VtblEntry *)self->base.base.vtable)[12];
  if (((int (*)(void *))vt->fn)((u8 *)self + vt->delta) != 0) {
    ActorStageObjAttrLandmarkSetIdle((ActorStageObjAttrLandmark *)self);
  }
  self->matrixDirty = 0;
  self->shaking = 1;
  self->shakePhase = 0;
}
