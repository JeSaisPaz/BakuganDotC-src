// bdc 0x088a41fc ActorStageObjEggCrystalOnHit
#include "bdc.h"

/* Hit reaction of the egg crystal (from `ActorStageObjEggCrystalUpdate` when its
   companion's collider registered a hit): takes the attacker from the companion unit's collider
   (`unit->collider0->hitAttacker`, stored in `hitBy`); if that attacker is a target point
   (`targetPointVariant` 2) it just re-arms the collider (`hitTimer = 1`, flag bit 0,
   `cooldown = 0`) and returns. Otherwise plays sound `0x20001f` at the model, spawns the hit effect
   (`ActorStageObjEggCrystalSpawnHitEffect`) at the hit point pulled 4 units toward the camera
   (along `g_gfxActiveCamera->dir`), pointing horizontally from the crystal to that point, computes
   the damage (`BtlCalcDamage``(attacker, hitParam164, hitKind, 4, 6)`, defaults 4 / 0x18
   without a unit) and applies it (`ActorStageObjApplyDamage`), restores a pending shake origin
   and, when HP is out, sets the debris velocity `debrisVel` (10 units along the attacker's yaw, or
   along the direction from the hit point to the crystal without an attacker), the dead flag and
   drops an item (`ActorStageObjDropItem`); otherwise starts the shake. Same shape as
   `ActorStageObjCrystalOnHit`. Without a unit the hit point is zero (bank C720). */

void ActorStageObjEggCrystalOnHit(ActorStageObjEggCrystal *self)
{
  float hitPos[4];
  float pos[4];
  float flat[4];
  float dir[4];
  const float *camDir;
  float lenSq;
  float invLen;
  float heading;
  s32 hitClass;
  s32 attackId;
  float damage;

  self->base.hitBy = NULL;
  if (self->unit != NULL) {
    self->base.hitBy = ((BtlBakugan *)self->unit)->collider0->hitAttacker;
  }
  if (self->base.hitBy != NULL && ((BtlBakugan *)self->base.hitBy)->targetPointVariant == 2) {
    if (self->unit != NULL) {
      CollisionCollider *collider = ((BtlBakugan *)self->unit)->collider0;
      collider->hitTimer = 1;
      collider->flags = collider->flags | 1;
      ((BtlBakugan *)self->unit)->collider0->cooldown = 0;
    }
    return;
  }

  /* hitPos = the collider's hit position, or zero (bank C720) without a unit. */
  hitPos[0] = 0.0f;
  hitPos[1] = 0.0f;
  hitPos[2] = 0.0f;
  hitPos[3] = 0.0f;
  if (self->unit != NULL) {
    const float *src = &((BtlBakugan *)self->unit)->collider0->hitPos.x;
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
  ActorStageObjEggCrystalSpawnHitEffect(self, pos, dir);

  hitClass = 4;
  attackId = 0x18;
  if (self->unit != NULL) {
    CollisionCollider *collider = ((BtlBakugan *)self->unit)->collider0;
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
    /* debrisVel = (cos, 0, sin) of heading (vrot with S703 = 2/pi) * 10, w = 0. */
    self->debrisVel[0] = __builtin_cosf(heading) * 10.0f;
    self->debrisVel[1] = 0.0f;
    self->debrisVel[2] = __builtin_sinf(heading) * 10.0f;
    self->debrisVel[3] = 0.0f;
    self->base.shakePhase = 0;
    self->base.dead = 1;
    ActorStageObjDropItem(&self->base);
  } else {
    self->base.shaking = 1;
    self->base.shakePhase = 0;
  }
}
