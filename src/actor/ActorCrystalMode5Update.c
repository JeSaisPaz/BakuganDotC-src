// bdc 0x08859488 ActorCrystalMode5Update
#include "bdc.h"

/* Crystal mode-5 handler (entry 5 of the mode table `0x08a670ec`): like `ActorCrystalMode4Update`
   but aims a shot from a point 250 units out along the facing angle and 250 up (`firePoint`), with
   direction `firePoint2` (unit vector to the target's head, scaled by 2.5), spawns the directional
   effect 0x30 attached to `firePoint`, and after 60 frames fires an attack object of type 0x7f from
   them (`BtlAttackCtor`, `BtlAttackLaunchStage`).
   Steps: 0 = range check (out of `attackRange` sets step 999), 0/1 = aim + effect + sound
   0x20025a, 2 = wait, 3 = fire. Returns 1 when the step is negative or past 3, else 0 (also 0
   while there is no usable target).
   The VFPU bank constants it read (S703 = 2/pi, S713 = 0) are literals here. */

int ActorCrystalMode5Update(ActorCrystal *self)

{
  float dir[4];
  float aim[4];
  BtlBakugan *target;
  BtlAttack *mem;
  BtlAttack *attack;
  GfxEffect *effect;
  SndListener *listener;
  float dx;
  float dy;
  float dz;
  float dist;
  float angle;
  float lenSq;
  float k;
  bool fromLow;
  int result;
  int step;
  int t;

  result = 0;
  target = (BtlBakugan *)ActorCrystalPickTarget(self);
  if (target == NULL || target->combat.dead != 0 || target->respawnProtect != 0) {
    return result;
  }
  step = self->step;
  if (step < 2) {
    if (step < 0) {
      return 1;
    }
    if (step <= 0) {
      dx = self->base.base.pos[0] - target->base.pos[0];
      dy = self->base.base.pos[1] - target->base.pos[1];
      dz = self->base.base.pos[2] - target->base.pos[2];
      dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
      if (!(dist < self->attackRange)) {
        self->step = 999;
        return result;
      }
      self->step = self->step + 1;
    }
    BtlBakuganSetTarget(&self->base, target);
    angle = atan2f(target->base.pos[2] - self->base.base.pos[2],
                   target->base.pos[0] - self->base.base.pos[0]);
    self->fireParam = angle;
    /* vrot [C,0,S,0] of angle * 2/pi (bank S703), scaled by 250 on x, y, z */
    dir[0] = __builtin_cosf(angle) * 250.0f;
    dir[1] = 0.0f;
    dir[2] = __builtin_sinf(angle) * 250.0f;
    dir[3] = 0.0f;
    self->firePoint[0] = dir[0] + self->base.base.pos[0];
    self->firePoint[1] = dir[1] + self->base.base.pos[1];
    self->firePoint[2] = dir[2] + self->base.base.pos[2];
    self->firePoint[3] = dir[3];
    self->firePoint[1] = self->firePoint[1] + 250.0f;
    if (SndHasListener()) {
      listener = SndGetListener();
      SndEmitterCreateAtPos(listener, 0x20025a, &self->base.base.data->rootMatrix[12], 0, 1);
    }
    effect = GfxEffectSpawnDirected(g_btlUnitEffectMgr, 0x30, self->firePoint, dir);
    effect->attachPos = self->firePoint;
    aim[0] = target->base.pos[0];
    aim[1] = target->base.pos[1];
    aim[2] = target->base.pos[2];
    aim[3] = target->base.pos[3];
    aim[1] = aim[1] + target->combat.stats->height;
    self->firePoint2[0] = aim[0] - self->firePoint[0];
    self->firePoint2[1] = aim[1] - self->firePoint[1];
    self->firePoint2[2] = aim[2] - self->firePoint[2];
    self->firePoint2[3] = aim[3];
    /* normalise (factor 0 for a zero vector, bank S713), clamp to [-1, 1]; w is S713 = 0 */
    lenSq = self->firePoint2[0] * self->firePoint2[0] + self->firePoint2[1] * self->firePoint2[1] +
            self->firePoint2[2] * self->firePoint2[2];
    k = VfRsq(lenSq);
    if (lenSq == 0.0f) {
      k = 0.0f;
    }
    self->firePoint2[0] = VfSat1(self->firePoint2[0] * k);
    self->firePoint2[1] = VfSat1(self->firePoint2[1] * k);
    self->firePoint2[2] = VfSat1(self->firePoint2[2] * k);
    self->firePoint2[3] = 0.0f;
    self->firePoint2[0] = self->firePoint2[0] * 2.5f;
    self->firePoint2[1] = self->firePoint2[1] * 2.5f;
    self->firePoint2[2] = self->firePoint2[2] * 2.5f;
    self->firePoint2[3] = 0.0f;
    self->timer = 0;
    self->step = self->step + 1;
    return result;
  }
  if (step < 3) {
    t = self->timer;
    self->timer = t + 1;
    if (t < 59) {
      return result;
    }
    self->step = self->step + 1;
  }
  else if (!(step < 4)) {
    return 1;
  }
  attack = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = (BtlAttack *)MemAlloc(0x160, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != NULL) {
    BtlAttackCtor(mem, self, 0x7f);
    attack = mem;
  }
  attack->param0 = self->attackParam0;
  attack->param1 = self->attackParam1;
  BtlAttackLaunchStage(attack, self->firePoint, self->firePoint2, NULL);
  self->step = self->step + 1;
  return result;
}
