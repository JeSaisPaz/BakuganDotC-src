// bdc 0x088b44c0 ActorStageObjCrystalOnHit
#include "bdc.h"

/* Hit reaction of the crystal stage object (from `ActorStageObjCrystalUpdate` when its
   companion's collider registered a hit): takes the attacker from the companion unit's collider
   (`unit->collider0->hitAttacker`, stored in `hitBy`); if that attacker is a target point
   (`targetPointVariant` 2) it just re-arms the collider (`hitTimer = 1`, flag bit 0,
   `cooldown = 0`) and returns. Otherwise plays sound `0x20001f` at the model, spawns the hit effect
   (`ActorStageObjCrystalSpawnHitEffect`) at the hit point pulled 4 units toward the camera
   (along `g_gfxActiveCamera->dir`), pointing horizontally from the crystal to that point, computes
   the damage (`BtlCalcDamage``(attacker, hitParam164, hitKind, 0, 6)`, defaults 4 / 0x18
   without a unit) and applies it (`ActorStageObjApplyDamage`), restores a pending shake origin
   and, when HP is out, sets the knock direction (10 units along the attacker's yaw, or along the
   direction from the hit point to the crystal without an attacker) and the dead flag; otherwise
   starts the shake. Without a unit the hit point is the origin (bank C720 = 0); a zero-length
   direction gets the inverse length 0 (bank S713). */

void ActorStageObjCrystalOnHit(ActorStageObjCrystal *self)
{
  float hitPos[4];
  float pos[4];
  float camOff[4];
  float rel[4];
  float flat[4];
  float effectPos[4];
  float dir[4];
  float hitCopy[4];
  float selfPos[4];
  float len2;
  float inv;
  float heading;
  s32 hitClass;
  s32 attackId;
  float damage;
  int i;

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

  /* hitPos = bank C720 (origin), or the collider's hit position when there is a unit. */
  hitPos[0] = 0.0f;
  hitPos[1] = 0.0f;
  hitPos[2] = 0.0f;
  hitPos[3] = 0.0f;
  if (self->unit != NULL) {
    ScePspFVector4 *hp = &((BtlBakugan *)self->unit)->collider0->hitPos;
    hitPos[0] = hp->x;
    hitPos[1] = hp->y;
    hitPos[2] = hp->z;
    hitPos[3] = hp->w;
  }
  if (SndHasListener()) {
    SndEmitterCreateAtPos(SndGetListener(), 0x20001f, &self->base.base.data->rootMatrix[12], 0, 1);
  }

  /* pos = hitPos - camera dir * 4 (xyz, w kept). */
  for (i = 0; i < 4; i++) {
    pos[i] = hitPos[i];
  }
  for (i = 0; i < 3; i++) {
    camOff[i] = g_gfxActiveCamera->dir[i] * 4.0f;
  }
  camOff[3] = 0.0f; /* lane 3 of C710 is the bank S713 */
  for (i = 0; i < 3; i++) {
    pos[i] = pos[i] - camOff[i];
  }
  /* rel = pos - crystal position (xyz, w = pos.w); flat = rel with y = 0. */
  for (i = 0; i < 3; i++) {
    rel[i] = pos[i] - self->base.base.pos[i];
  }
  rel[3] = pos[3];
  for (i = 0; i < 4; i++) {
    flat[i] = rel[i];
  }
  flat[1] = 0.0f;

  /* flat = normalize3(flat), clamped to [-1, 1]; inverse length 0 for a zero vector. */
  len2 = flat[0] * flat[0] + flat[1] * flat[1] + flat[2] * flat[2];
  inv = VfRsq(len2);
  if (len2 == 0.0f) {
    inv = 0.0f;
  }
  for (i = 0; i < 3; i++) {
    flat[i] = VfSat1(flat[i] * inv);
  }
  flat[3] = 0.0f; /* masked lane of C710: the bank S713 */

  for (i = 0; i < 4; i++) {
    effectPos[i] = pos[i];
    dir[i] = flat[i];
  }
  ActorStageObjCrystalSpawnHitEffect(self, effectPos, dir);

  hitClass = 4;
  attackId = 0x18;
  if (self->unit != NULL) {
    CollisionCollider *collider = ((BtlBakugan *)self->unit)->collider0;
    hitClass = collider->hitParam164;
    attackId = collider->hitKind;
  }
  damage = BtlCalcDamage(self->base.hitBy, hitClass, attackId, 0, 6);
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
      for (i = 0; i < 4; i++) {
        hitCopy[i] = hitPos[i];
        selfPos[i] = self->base.base.pos[i];
      }
      heading = atan2f(selfPos[2] - hitCopy[2], selfPos[0] - hitCopy[0]);
    }
    /* knockDir = (cos, 0, sin) of heading * 10, w = 0 (vrot of heading * S703). */
    self->knockDir[0] = __builtin_cosf(heading) * 10.0f;
    self->knockDir[1] = 0.0f * 10.0f;
    self->knockDir[2] = __builtin_sinf(heading) * 10.0f;
    self->knockDir[3] = 0.0f;
    self->base.shakePhase = 0;
    self->base.dead = 1;
  } else {
    self->base.shaking = 1;
    self->base.shakePhase = 0;
  }
}
