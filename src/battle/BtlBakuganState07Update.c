// bdc 0x0886c5b4 BtlBakuganState07Update
#include "bdc.h"

/* Per-frame handler of Bakugan battle state 7 (`+0x140`, also run for state 9), vtable slot
   `+0x108` called through `BtlBakuganRunState`: the melee combo attack. Returns to state 0 at
   once when there is no current combo, motion set or step (`comboStep` -1), and returns when
   `BtlBakuganTryCancelIntoArt` switched to an art. Otherwise it counts `attackFrame` (and
   `attackEventFrame` from phase 2), works out whether a follow-up step is queued and runs the
   current `attackPhase` (motions of `BtlBakuganPlayComboStepMotion`):
   0 wind-up: at progress `attackAdvanceProgress` goes to the approach loop (phase 1) when the
     target is not near, else straight to the strike (phase 2), then sets the combo motion speed;
   1 approach: after 9 frames or once near (or without target) plays the strike;
   2 strike: sets the counter windows, without a target pushes the unit forward (state 9: along its
     heading at 0.8 x `BtlBakuganGetScaledStat48`), waits out the strike loop count and, at
     `attackAdvanceProgress`, arms the hit window and plays the end motion (phase 3; none: state 0);
   4 end: the attack command (0x10) with energy against an airborne target (state 4 with `flags` 2) restarts the attack
     (`BtlBakuganStartAttackOrArt`) and returns; else continues as 3;
   3 end: from `comboChainProgress` chains the next queued step (or the next combo in state 9) or
     returns to state 0 at the end of the motion, and starts the slow-motion finisher
     (`BtlSlowMotionTaskCreate`) when a final step landed a hit in a player fight.
   Phases 1..3 (3 only with motion-set flag 4) home on the target (velocity toward its position, vertical speed damped) and,
   when not homing, the horizontal velocity is damped by 0.83.
   The VFPU bank constants read are written as literals: 2/π (S703) turns `rot[1]` into the
   quarter turns of `vrot.q` (so cos/sin of the yaw), and 0 (S713) is the inverse length used for
   a zero aim vector and the w lane of the scaled aim copied into `velocity`. */

/* Virtual slot 6 (motion speed; `BtlBakuganSetMotionSpeed` for the base class). */
static inline void BtlBakuganCallSetMotionSpeed(BtlBakugan *self, float speed)
{
  const VtblEntry *entry = &((const VtblEntry *)self->base.base.vtable)[6];

  ((float (*)(float, void *))entry->fn)(speed, (u8 *)self + entry->delta);
}

/* Virtual slot 26 (state 0 handler; `BtlBakuganState00Update` for the base class). */
static inline void BtlBakuganCallState00Update(BtlBakugan *self)
{
  const VtblEntry *entry = &((const VtblEntry *)self->base.base.vtable)[26];

  ((void (*)(void *))entry->fn)((u8 *)self + entry->delta);
}

/* aim.xyz *= len / |aim.xyz| (0 instead of the inverse length when the length is 0); the result is
   built in C710 whose w lane is the bank zero S713, so aim.w becomes 0. */
static inline void BtlBakuganScaleAim(float *aim, float len)
{
  float lenSq;
  float inv;
  float k;

  lenSq = aim[0] * aim[0] + aim[1] * aim[1] + aim[2] * aim[2];
  inv = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
  k = inv * len;
  aim[0] = aim[0] * k;
  aim[1] = aim[1] * k;
  aim[2] = aim[2] * k;
  aim[3] = 0.0f;
}

/* BtlBakuganScaleAim, then the scaled aim (all four lanes) becomes the velocity. */
static inline void BtlBakuganAimVelocity(float *aim, float *velocity, float len)
{
  BtlBakuganScaleAim(aim, len);
  velocity[0] = aim[0];
  velocity[1] = aim[1];
  velocity[2] = aim[2];
  velocity[3] = aim[3];
}

void BtlBakuganState07Update(BtlBakugan *self)
{
  BtlComboStep *combo;
  BtlAttackMotionSet *set;
  BtlBakugan *target;
  float *velocity;
  float advanceProgress;
  float chainProgress;
  float endProgress;
  float framesLeft;
  float window;
  float yaw;
  float speed;
  float hsq;
  float dist;
  float gap;
  float vy;
  float dash;
  float scale;
  float keep;
  s32 wait;
  s32 match;
  int homing;
  int chainQueued;
  int ended;
  int playedNext;
  u8 freshApproach;
  float aim[4];

  combo = self->combos[self->comboIndex];
  set = (BtlAttackMotionSet *)BtlBakuganGetComboStepMotionSet(self);
  endProgress = 1.0f;
  BtlBakuganTurnTowardTarget(self, NULL);
  target = (BtlBakugan *)BtlBakuganGetTarget(self);
  homing = target != NULL;
  if (combo == NULL || set == NULL || self->comboStep == -1) {
    BtlBakuganSetState(self, 0, 0);
    return;
  }
  if (BtlBakuganTryCancelIntoArt(self) != 0) {
    return;
  }
  self->attackFrame++;
  if ((s32)self->attackPhase >= 2) {
    self->attackEventFrame++;
  }
  self->finalStepPlaying = 0;
  self->forceFinalHit = 0;
  self->stateFlags |= 0x400000;
  chainQueued = self->queuedComboSteps != 0 && BtlBakuganGetNextComboStepMotionSet(self) != NULL;
  chainProgress = self->combat.stats->comboChainProgress;
  advanceProgress = self->combat.stats->attackAdvanceProgress;
  if (set->flags & 2) {
    self->stateFlags |= 0x8000000;
  }
  self->foldAttackIds = 0;
  if (BtlBakuganGetNextComboStepMotionSet(self) != NULL && self->queuedComboSteps > 0) {
    self->foldAttackIds = 1;
  }
  self->counterOpening = 0;
  freshApproach = 0;
  if (self->state == 9) {
    self->counterOpening = 1;
  }
  velocity = self->base.velocity;

  switch (self->attackPhase) {
  case 0:
    if (GfxModelMotionReached(&self->base, advanceProgress)) {
      if (self->targetNear == 0 && BtlBakuganGetTarget(self) != NULL) {
        if (BtlBakuganPlayComboStepMotion(self, 1) != 0) {
          self->subWait = 0;
        } else {
          BtlBakuganPlayComboStepMotion(self, 2);
        }
        freshApproach = 1;
      } else {
        BtlBakuganPlayComboStepMotion(self, 2);
      }
      BtlBakuganCallSetMotionSpeed(self, self->combat.stats->comboMotionSpeed);
    }
    break;

  case 1:
    wait = self->subWait;
    self->subWait = wait + 1;
    if (!(wait < 9 && self->targetNear == 0 && BtlBakuganGetTarget(self) != NULL)) {
      BtlBakuganPlayComboStepMotion(self, 2);
      BtlBakuganCallSetMotionSpeed(self, self->combat.stats->comboMotionSpeed);
    }
    self->stateFlags |= 0x1000000;
    break;

  case 2:
    if (target != NULL && self->comboStep < 3 && GfxModelMotionFrame(&self->base) < 3.0f) {
      self->flags |= 0x200;
    }
    window = SaveGetProfileFlag0() ? 4.0f : 8.0f;
    if (self->comboStep > 0 && self->comboStep < 3 &&
        !(GfxModelMotionFrame(&self->base) < 0.0f) && GfxModelMotionFrame(&self->base) <= window) {
      self->counterOpening = 1;
    }
    self->flags |= 0x40;
    if (!homing) {
      if (self->state == 9) {
        yaw = self->base.rot[1];
        speed = BtlBakuganGetScaledStat48(self) * 0.800000012f;
        /* vrot.q [C,0,S,0] of yaw * 2/π, scaled by speed (lanes 0..2), stored as the velocity */
        velocity[0] = __builtin_cosf(yaw) * speed;
        velocity[1] = 0.0f * speed;
        velocity[2] = __builtin_sinf(yaw) * speed;
        velocity[3] = 0.0f;
      } else if (self->subTimer == 0) {
        /* horizontal speed squared: vdot.t with the y lane zeroed */
        hsq = velocity[0] * velocity[0] + velocity[2] * velocity[2];
        speed = BtlBakuganGetMeleeStepSpeed(self);
        if (hsq < speed * BtlBakuganGetMeleeStepSpeed(self)) {
          yaw = self->base.rot[1];
          speed = BtlBakuganGetMeleeStepSpeed(self);
          velocity[0] = velocity[0] + __builtin_cosf(yaw) * speed;
          velocity[2] = velocity[2] + __builtin_sinf(yaw) * speed;
          velocity[1] = 0.0f;
        } else {
          self->subTimer = 1;
        }
      }
    }
    if ((s32)self->attackMotionFlags > 0) {
      if (self->base.motionEnded) {
        self->base.motionEnded = 0;
        self->attackMotionFlags--;
      }
      break;
    }
    if (GfxModelMotionReached(&self->base, advanceProgress)) {
      if (!(set->flags & 0x10000)) {
        BtlBakuganStartHitWindow(self, BtlBakuganGetComboStepParam(self));
      }
      self->subTimer = 0;
      self->finalStepPlaying = 1;
      if (BtlBakuganPlayComboStepMotion(self, 3) == 0) {
        if (self->state == 9) {
          BtlBakuganSetState(self, 0, 0);
        } else {
          BtlBakuganSetState(self, 0, 0);
          BtlBakuganCallState00Update(self);
        }
        if (self->slowMotionActive) {
          BtlSlowMotionTaskRemove();
          self->slowMotionActive = 0;
        }
        keep = BtlScaleRetentionByTimeStep(0.349999994f);
        velocity[1] = velocity[1] * keep;
      } else {
        if (set->flags & 4) {
          velocity[1] = self->gravity * 8.0f;
        }
        BtlBakuganCallSetMotionSpeed(self, self->combat.stats->comboMotionSpeed);
      }
    }
    break;

  case 4:
    self->stateFlags &= ~0x8000000u;
    if ((self->commands & 0x10) && BtlCombatHasEnergy(&self->combat) != 0 && homing &&
        target->state == 4 && (target->flags & 2)) {
      BtlBakuganStartAttackOrArt(self, 0);
      self->flags |= 4;
      if (self->slowMotionActive) {
        BtlSlowMotionTaskRemove();
        self->slowMotionActive = 0;
      }
      return;
    }
    /* fall through */
  case 3:
    if (self->attackPhase == 3) {
      self->flags |= 0x40;
    }
    self->finalStepPlaying = 1;
    if (self->comboStep < 3 && target != NULL) {
      framesLeft = GfxModelGetMotionEnd(&self->base);
      framesLeft = (chainProgress - GfxModelGetMotionProgress(&self->base)) * framesLeft;
      window = SaveGetProfileFlag0() ? 9.0f : 6.0f;
      if (framesLeft <= window) {
        self->flags |= 0x200;
      }
      if (self->comboStep < 2 && SaveGetProfileFlag0() && framesLeft <= 56.0f) {
        self->counterOpening = 1;
      }
    }
    if (self->comboStep >= 3 && homing && target->state == 4 && (target->flags & 2)) {
      chainProgress = chainProgress * 0.699999988f;
      endProgress = endProgress * 0.699999988f;
    }
    if (!GfxModelMotionReached(&self->base, chainProgress) && !self->base.motionEnded) {
      if (self->slowMotionActive) {
        BtlSlowMotionTaskRemove();
        self->slowMotionActive = 0;
      }
      break;
    }
    self->stateFlags &= ~0x8000000u;
    ended = 0;
    playedNext = 0;
    if (self->state == 9 && !chainQueued && self->comboIndex < 6 &&
        self->combos[self->comboIndex + 2] != NULL) {
      /* state 9: no step queued, move on to the combo two entries further */
      self->comboStep = 0;
      self->comboIndex += 2;
      self->comboAirVariant = 0;
      BtlBakuganPlayComboStepMotion(self, 2);
      BtlBakuganCallSetMotionSpeed(self, self->combat.stats->comboMotionSpeed);
      self->attackFrame = 1;
      self->attackEventFrame = 1;
      break;
    }
    if ((GfxModelMotionReached(&self->base, endProgress) || self->base.motionEnded) &&
        !chainQueued) {
      ended = 1;
      if (self->attackPhase == 3 && !(set->flags & 0x8000) &&
          BtlBakuganPlayComboStepMotion(self, 4) != 0) {
        ended = 0;
      }
    } else if (chainQueued) {
      self->comboStep++;
      match = BtlBakuganMatchComboStepInput(self);
      if (match == 1) {
        self->comboAirVariant = 1;
      }
      self->attackFrame = 0;
      self->queuedComboSteps--;
      self->attackEventFrame = 0;
      if (match == -1) {
        self->comboStep = BtlBakuganGetComboLength(self);
        ended = GfxModelMotionReached(&self->base, endProgress) || self->base.motionEnded;
      } else if (BtlBakuganPlayComboStepMotion(self, 2) == 0) {
        ended = GfxModelMotionReached(&self->base, endProgress) || self->base.motionEnded;
      } else {
        BtlBakuganCallSetMotionSpeed(self, self->combat.stats->comboMotionSpeed);
        playedNext = 1;
      }
      BtlBakuganCountAttackStat(self);
    } else {
      ended = GfxModelMotionReached(&self->base, endProgress) || self->base.motionEnded;
    }
    if (ended) {
      if (self->state == 9) {
        BtlBakuganSetState(self, 0, 0);
      }
      BtlBakuganSetState(self, 0, 0);
      keep = BtlScaleRetentionByTimeStep(0.349999994f);
      velocity[1] = velocity[1] * keep;
      if (self->slowMotionActive) {
        BtlSlowMotionTaskRemove();
        self->slowMotionActive = 0;
      }
    } else if (self->motionHitLanded && self->comboStep >= 3 &&
               BtlBakuganGetNextComboStepMotionSet(self) == NULL && self->state == 7 &&
               target != NULL && BtlBakuganTargetInvolvesPlayer(self) != 0 && playedNext) {
      BtlSlowMotionTaskCreate(self, target, 0);
      self->slowMotionActive = 1;
    } else {
      BtlSlowMotionTaskRemove();
      self->slowMotionActive = 0;
    }
    self->motionHitLanded = 0;
    break;

  default:
    break;
  }

  if ((self->state == 9 || self->state == 7) && self->attackPhase != 1) {
    BtlBakuganSetStateFlag04(self);
    if (self->attackPhase == 2) {
      self->attackResult = 1;
    }
  }
  if (!(set->flags & 4) && self->attackPhase == 3) {
    homing = 0;
  }
  if (self->attackPhase != 2 && self->attackPhase != 1 && self->attackPhase != 3) {
    homing = 0;
  } else {
    if (homing && (chainQueued || (s32)self->attackPhase < 3)) {
      /* aim = target position (height lowered by its hover height, not below its ground) - pos */
      aim[0] = target->base.pos[0];
      aim[1] = target->base.pos[1];
      aim[2] = target->base.pos[2];
      aim[3] = target->base.pos[3];
      aim[1] = aim[1] - target->combat.stats->hoverHeight;
      if (aim[1] < target->groundY) {
        aim[1] = target->groundY;
      }
      aim[0] = aim[0] - self->base.pos[0];
      aim[1] = aim[1] - self->base.pos[1];
      aim[2] = aim[2] - self->base.pos[2];
      if (set->flags & 0x80) {
        BtlBakuganAimVelocity(aim, velocity, self->dashSpeed);
        self->dashSpeed = self->dashSpeed * 0.850000024f;
      } else if (freshApproach || self->attackFrame == 1) {
        if ((target->stateFlags & 0x40000) && (target->flags & 2)) {
          speed = BtlBakuganGetMeleeLungeSpeed(self);
        } else {
          speed = BtlBakuganGetMeleeStepSpeed(self);
        }
        self->dashSpeed = speed;
        BtlBakuganAimVelocity(aim, velocity, speed);
        vy = velocity[1];
        if (!(vy <= 30.0f)) {
          vy = 30.0f;
        } else if (vy < -30.0f) {
          vy = -30.0f;
        }
        velocity[1] = vy;
      } else {
        dist = __builtin_sqrtf(aim[0] * aim[0] + aim[1] * aim[1] + aim[2] * aim[2]);
        gap = dist - self->combat.stats->reachRadius * 1.29999995f;
        speed = self->dashSpeed;
        if (gap < 0.0f) {
          gap = 0.0f;
        } else if (speed < gap) {
          gap = speed;
        }
        BtlBakuganScaleAim(aim, gap);
        /* velocity += (aim - velocity) * 0.3, all four lanes */
        velocity[0] = velocity[0] + (aim[0] - velocity[0]) * 0.300000012f;
        velocity[1] = velocity[1] + (aim[1] - velocity[1]) * 0.300000012f;
        velocity[2] = velocity[2] + (aim[2] - velocity[2]) * 0.300000012f;
        velocity[3] = velocity[3] + (aim[3] - velocity[3]) * 0.300000012f;
        if ((target->stateFlags & 0x40000) && (target->flags & 2)) {
          if ((s32)self->attackPhase < 3) {
            self->dashSpeed = BtlBakuganGetMeleeLungeSpeed(self);
            vy = BtlCalcDeceleratingDistance(target->base.velocity[1],
                                             target->combat.stats->gravity * 0.75f,
                                             (u32)(uintptr_t)self, 6); /* bdc: ptr-narrow ok: the callee never reads its third argument */
            vy = (vy + target->base.pos[1]) - (self->base.pos[1] + velocity[1]);
            if (!(vy <= self->dashSpeed)) {
              vy = self->dashSpeed;
            }
            velocity[1] = vy;
          }
        } else if (target->state == 3 && (self->flags & 4)) {
          velocity[1] = ((target->base.pos[1] - self->base.pos[1]) - velocity[1]) * 0.200000003f;
        }
        dash = self->dashSpeed;
        scale = GfxGetMotionTimeScale();
        speed = BtlBakuganGetMeleeStepSpeed(self);
        self->dashSpeed = dash + (speed * 0.699999988f - self->dashSpeed) * 0.100000001f * scale;
      }
    }
    if (target != NULL) {
      if (!homing) {
        velocity[1] = velocity[1] * 0.850000024f;
      } else if (self->base.pos[1] < target->base.pos[1]) {
        if (velocity[1] < 0.0f) {
          velocity[1] = velocity[1] * 0.300000012f;
        }
      } else if (!(velocity[1] <= 0.0f)) {
        velocity[1] = velocity[1] * 0.300000012f;
      }
    }
  }
  if (!homing) {
    /* horizontal damping: velocity.x/z *= 0.83 */
    velocity[0] = velocity[0] * 0.829999983f;
    velocity[2] = velocity[2] * 0.829999983f;
  }
}
