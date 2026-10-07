// bdc 0x08859124 ActorCrystalMode4Update
#include "bdc.h"

/* Crystal mode-4 handler (entry 4 of the mode table `0x08a670ec`, see `ActorCrystalUpdateLogic`):
   picks a target (`ActorCrystalPickTarget`); step 0 stays put (returns 0) while the target is
   not closer than `attackRange`. Steps 0/1 play the launch sound 0x20001e (`SndEmitterCreateAtPos`),
   turn toward the target (`BtlBakuganSetTarget`, `atan2f` into `fireParam`) and spawn the muzzle
   effect 0x1c 250 units out along that angle and 125 up; step 2 waits 22 frames; step 3 fires an
   attack object of type 0x7e (`BtlAttackCtor`, launched with `BtlAttackLaunchStage`) from the
   same muzzle point toward the target's head (unit direction scaled by 2.5).
   Returns 1 when the step is negative or past 3, else 0 (also 0 while there is no usable target).
   The VFPU bank constants it read (S703 = 2/pi, S713 = 0) are literals here. */

int ActorCrystalMode4Update(ActorCrystal *self)
{
  float pos[4];
  float dir[4];
  float shotDir[4];
  float aim[4];
  float from[4];
  BtlBakugan *target;
  BtlAttack *mem;
  BtlAttack *attack;
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
        return result;
      }
      self->step = self->step + 1;
    }
    if (SndHasListener()) {
      listener = SndGetListener();
      SndEmitterCreateAtPos(listener, 0x20001e, &self->base.base.data->rootMatrix[12], 0, 1);
    }
    BtlBakuganSetTarget(&self->base, target);
    angle = atan2f(target->base.pos[2] - self->base.base.pos[2],
                   target->base.pos[0] - self->base.base.pos[0]);
    self->fireParam = angle;
    /* vrot [C,0,S,0] of angle * 2/pi (bank S703) */
    dir[0] = __builtin_cosf(angle);
    dir[1] = 0.0f;
    dir[2] = __builtin_sinf(angle);
    dir[3] = 0.0f;
    /* same rotation of the reloaded fireParam, scaled by 250 on x, y, z, plus the position */
    angle = self->fireParam;
    pos[0] = __builtin_cosf(angle) * 250.0f + self->base.base.pos[0];
    pos[1] = 0.0f * 250.0f + self->base.base.pos[1];
    pos[2] = __builtin_sinf(angle) * 250.0f + self->base.base.pos[2];
    pos[3] = 0.0f;
    pos[1] = pos[1] + 125.0f;
    GfxEffectSpawnDirected(g_btlUnitEffectMgr, 0x1c, pos, dir);
    self->timer = 0;
    self->step = self->step + 1;
    return result;
  }
  if (step < 3) {
    t = self->timer;
    self->timer = t + 1;
    if (t < 21) {
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
    BtlAttackCtor(mem, self, 0x7e);
    attack = mem;
  }
  aim[0] = target->base.pos[0];
  aim[1] = target->base.pos[1];
  aim[2] = target->base.pos[2];
  aim[3] = target->base.pos[3];
  aim[1] = aim[1] + target->combat.stats->height;
  angle = self->fireParam;
  from[0] = __builtin_cosf(angle) * 250.0f + self->base.base.pos[0];
  from[1] = 0.0f * 250.0f + self->base.base.pos[1];
  from[2] = __builtin_sinf(angle) * 250.0f + self->base.base.pos[2];
  from[3] = 0.0f;
  from[1] = from[1] + 125.0f;
  shotDir[0] = aim[0] - from[0];
  shotDir[1] = aim[1] - from[1];
  shotDir[2] = aim[2] - from[2];
  /* normalise (factor 0 for a zero vector, bank S713), clamp to [-1, 1]; w is S713 = 0 */
  lenSq = shotDir[0] * shotDir[0] + shotDir[1] * shotDir[1] + shotDir[2] * shotDir[2];
  k = VfRsq(lenSq);
  if (lenSq == 0.0f) {
    k = 0.0f;
  }
  shotDir[0] = VfSat1(shotDir[0] * k) * 2.5f;
  shotDir[1] = VfSat1(shotDir[1] * k) * 2.5f;
  shotDir[2] = VfSat1(shotDir[2] * k) * 2.5f;
  shotDir[3] = 0.0f;
  attack->param0 = self->attackParam0;
  attack->param1 = self->attackParam1;
  BtlAttackLaunchStage(attack, from, shotDir, NULL);
  self->step = self->step + 1;
  return result;
}
