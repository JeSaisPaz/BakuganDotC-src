// bdc 0x088b4fd0 ActorStageObjCrystalState01Shoot
#include "bdc.h"

/* State 1 (shoot) of the crystal stage object, a step machine on `step`, aimed at the player's
   Bakugan (`BtlGetPlayerBakugan`; does nothing during cut-ins, without a player Bakugan or when
   it is dead): step 0 requires the player within 2000 units (else step 999, which the next frame
   turns back into state 0); steps 0/1 face the player (`heading`, `atan2f`), place the muzzle 50
   units out along the heading and 80 up, play sound `0x200033` at the model, spawn the charge
   effect 0x39 attached to the muzzle (`GfxEffectSpawnDirected`, `attachPos = muzzle`), store
   the shot direction (normalised, from the muzzle toward the player's position raised by its
   stat height, times 1.5; a zero-length direction stays zero), clear `shotTimer` and advance;
   step 2 waits until `shotTimer` has counted 60 frames; step 3 (entered the same frame) creates
   a 0x160-byte attack 0x83 owned by the companion `unit` (`BtlAttackCtor`, from the low heap),
   plays `0x200033` at the muzzle and launches it (`BtlAttackLaunchStage`, NULL attack if the
   allocation failed) and advances; a negative step or any step from 4 on returns to state 0
   (`ActorStageObjCrystalSetState`). */

void ActorStageObjCrystalState01Shoot(ActorStageObjCrystal *self)
{
  float tmp[4];
  float offs[4];
  float target[4];
  float d[3];
  float dist;
  float heading;
  float c;
  float s;
  float inv;
  BtlBakugan *player;
  GfxEffect *effect;
  BtlAttack *mem;
  BtlAttack *attack;
  bool fromLow;
  s32 step;
  s32 timer;

  player = (BtlBakugan *)BtlGetPlayerBakugan();
  if (BtlIsCutInRunning()) {
    return;
  }
  if (player == NULL) {
    return;
  }
  if (player->combat.dead != 0) {
    return;
  }
  step = self->step;
  if (step < 2) {
    if (step < 0) {
      ActorStageObjCrystalSetState(self, 0);
      return;
    }
    if (step <= 0) {
      /* dist = |self pos - player pos| (xyz). */
      d[0] = self->base.base.pos[0] - player->base.pos[0];
      d[1] = self->base.base.pos[1] - player->base.pos[1];
      d[2] = self->base.base.pos[2] - player->base.pos[2];
      dist = __builtin_sqrtf(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
      if (!(dist < 2000.0f)) {
        self->step = 999;
        return;
      }
      self->step = self->step + 1;
    }
    tmp[0] = player->base.pos[0];
    tmp[2] = player->base.pos[2];
    heading = atan2f(tmp[2] - self->base.base.pos[2], tmp[0] - self->base.base.pos[0]);
    self->heading = heading;
    /* offs = (cos, 0, sin)(heading) * 50 (vrot of heading * 2/pi); muzzle = offs + self pos. */
    c = __builtin_cosf(heading);
    s = __builtin_sinf(heading);
    offs[0] = c * 50.0f;
    offs[1] = 0.0f * 50.0f;
    offs[2] = s * 50.0f;
    offs[3] = 0.0f;
    self->muzzle[0] = offs[0] + self->base.base.pos[0];
    self->muzzle[1] = offs[1] + self->base.base.pos[1];
    self->muzzle[2] = offs[2] + self->base.base.pos[2];
    self->muzzle[3] = offs[3];
    self->muzzle[1] = self->muzzle[1] + 80.0f;
    if (SndHasListener()) {
      SndEmitterCreateAtPos(SndGetListener(), 0x200033, &self->base.base.data->rootMatrix[12], 0, 1);
    }
    effect = GfxEffectSpawnDirected(g_btlUnitEffectMgr, 0x39, self->muzzle, offs);
    effect->attachPos = self->muzzle;
    /* target = player pos, raised by its stat height. */
    target[0] = player->base.pos[0];
    target[1] = player->base.pos[1];
    target[2] = player->base.pos[2];
    target[3] = player->base.pos[3];
    target[1] = target[1] + player->combat.stats->height;
    /* shotDir = target - muzzle (w = target w), then normalised with each lane clamped to
       [-1, 1] (0 instead of 1/sqrt(0)) and w = 0, then times 1.5. */
    self->shotDir[0] = target[0] - self->muzzle[0];
    self->shotDir[1] = target[1] - self->muzzle[1];
    self->shotDir[2] = target[2] - self->muzzle[2];
    self->shotDir[3] = target[3];
    d[0] = self->shotDir[0];
    d[1] = self->shotDir[1];
    d[2] = self->shotDir[2];
    inv = d[0] * d[0] + d[1] * d[1] + d[2] * d[2];
    if (inv == 0.0f) {
      inv = 0.0f;
    } else {
      inv = VfRsq(inv);
    }
    self->shotDir[0] = VfSat1(d[0] * inv);
    self->shotDir[1] = VfSat1(d[1] * inv);
    self->shotDir[2] = VfSat1(d[2] * inv);
    self->shotDir[3] = 0.0f;
    self->shotDir[0] = self->shotDir[0] * 1.5f;
    self->shotDir[1] = self->shotDir[1] * 1.5f;
    self->shotDir[2] = self->shotDir[2] * 1.5f;
    self->shotDir[3] = 0.0f;
    self->shotTimer = 0;
    self->step = self->step + 1;
    return;
  }
  if (step < 3) {
    timer = self->shotTimer;
    self->shotTimer = timer + 1;
    if (timer < 0x3b) {
      return;
    }
    self->step = self->step + 1;
  } else if (!(step < 4)) {
    ActorStageObjCrystalSetState(self, 0);
    return;
  }

  attack = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = (BtlAttack *)MemAlloc(sizeof(BtlAttack), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != NULL) {
    BtlAttackCtor(mem, self->unit, 0x83);
    attack = mem;
  }
  if (SndHasListener()) {
    SndEmitterCreateAtPos(SndGetListener(), 0x200033, self->muzzle, 0, 1);
  }
  BtlAttackLaunchStage(attack, self->muzzle, self->shotDir, NULL);
  self->step = self->step + 1;
}
