// bdc 0x088a2b74 ActorStageObjLandmarkOnHit
#include "bdc.h"

/* Hit reaction of the landmark, called from `ActorStageObjLandmarkState00Active` when its
   companion collider was hit: takes the attacker from the companion unit's collider
   (`unit->collider0->hitAttacker`, stored in `hitBy`). If that attacker is a target point
   (`targetPointVariant` 2) it only re-arms the collider (`hitTimer = 1`, flag bit 0,
   `cooldown = 0`; nothing without a unit) and returns; while the helper collider exists it
   likewise only re-arms the unit's collider and returns. Otherwise plays sound `0x20001f` at the
   model, spawns the hit effect (`ActorStageObjLandmarkSpawnHitEffect`) at the hit point pulled
   4 units toward the camera (along `g_gfxActiveCamera->dir`), pointing horizontally from the
   landmark to that point, computes the damage (`BtlCalcDamage``(attacker, hitParam164,
   hitKind, 4, 6)`, defaults 4 / 0x18 without a unit) and applies it
   (`ActorStageObjApplyDamage`), restores a pending shake origin and, when HP is out, sets the
   knock direction (10 units along the attacker's yaw, or along the direction from the hit point
   to the landmark without an attacker) and the dead flag; otherwise starts the shake. Without a
   unit the hit point is zero (bank C720). */

void ActorStageObjLandmarkOnHit(ActorStageObjLandmark *self)
{
  float hitPos[4];
  float pos[4];
  float flat[4];
  float dir[4];
  const float *camDir;
  CollisionCollider *collider;
  float lenSq;
  float invLen;
  float heading;
  s32 hitClass;
  s32 attackId;
  float damage;

  self->base.hitBy = NULL;
  if (self->unit != NULL) {
    self->base.hitBy = self->unit->base.collider0->hitAttacker;
  }
  if (self->base.hitBy != NULL && ((BtlBakugan *)self->base.hitBy)->targetPointVariant == 2) {
    if (self->unit != NULL) {
      collider = self->unit->base.collider0;
      collider->hitTimer = 1;
      collider->flags = collider->flags | 1;
      self->unit->base.collider0->cooldown = 0;
    }
    return;
  }
  if (self->helper != NULL) {
    /* No unit check on this path. */
    collider = self->unit->base.collider0;
    collider->hitTimer = 1;
    collider->flags = collider->flags | 1;
    self->unit->base.collider0->cooldown = 0;
    return;
  }

  /* hitPos = the collider's hit position, or zero (bank C720) without a unit. */
  hitPos[0] = 0.0f;
  hitPos[1] = 0.0f;
  hitPos[2] = 0.0f;
  hitPos[3] = 0.0f;
  if (self->unit != NULL) {
    const float *src = &self->unit->base.collider0->hitPos.x;
    hitPos[0] = src[0];
    hitPos[1] = src[1];
    hitPos[2] = src[2];
    hitPos[3] = src[3];
  }
  if (SndHasListener()) {
    SndEmitterCreateAtPos(SndGetListener(), 0x20001f, &self->base.base.data->rootMatrix[12], 0, 1);
  }
  /* pos = hitPos pulled 4 units along the camera direction (w kept). */
  camDir = g_gfxActiveCamera->dir;
  pos[0] = hitPos[0] - camDir[0] * 4.0f;
  pos[1] = hitPos[1] - camDir[1] * 4.0f;
  pos[2] = hitPos[2] - camDir[2] * 4.0f;
  pos[3] = hitPos[3];
  /* flat = (pos - self pos) with y = 0, normalised (0 for a zero length), clamped, w = 0. */
  flat[0] = pos[0] - self->base.base.pos[0];
  flat[1] = 0.0f;
  flat[2] = pos[2] - self->base.base.pos[2];
  lenSq = flat[0] * flat[0] + flat[1] * flat[1] + flat[2] * flat[2];
  invLen = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
  dir[0] = VfSat1(flat[0] * invLen);
  dir[1] = VfSat1(flat[1] * invLen);
  dir[2] = VfSat1(flat[2] * invLen);
  dir[3] = 0.0f;
  ActorStageObjLandmarkSpawnHitEffect(self, pos, dir);

  hitClass = 4;
  attackId = 0x18;
  if (self->unit != NULL) {
    collider = self->unit->base.collider0;
    hitClass = collider->hitParam164;
    attackId = collider->hitKind;
  }
  damage = BtlCalcDamage(self->base.hitBy, hitClass, attackId, 4, 6);
  ActorStageObjApplyDamage(damage, &self->base);
  if (self->base.shaking != 0) {
    self->base.base.pos[0] = self->base.shakeX;
    self->base.base.pos[2] = self->base.shakeZ;
    self->base.shaking = 0;
    self->base.shakePhase = 0;
  }
  if ((float)self->base.hp <= 0.0f) {
    if (self->base.hitBy != NULL) {
      heading = ((BtlBakugan *)self->base.hitBy)->base.rot[1];
    } else {
      heading = atan2f(self->base.base.pos[2] - hitPos[2], self->base.base.pos[0] - hitPos[0]);
    }
    /* knockDir = (cos, 0, sin) of heading (vrot with S703 = 2/pi) * 10, w = 0. */
    self->knockDir[0] = __builtin_cosf(heading) * 10.0f;
    self->knockDir[1] = 0.0f;
    self->knockDir[2] = __builtin_sinf(heading) * 10.0f;
    self->knockDir[3] = 0.0f;
    self->base.shakePhase = 0;
    self->base.dead = 1;
  } else {
    self->base.shaking = 1;
    self->base.shakePhase = 0;
  }
}
