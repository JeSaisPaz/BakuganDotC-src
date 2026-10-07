// bdc 0x0886d688 BtlBakuganState08Update
#include "bdc.h"

/* Per-frame handler of Bakugan battle state 8 (`+0x140`), vtable slot `+0x110` called through
   `BtlBakuganRunState`: the plain attack (shares `BtlBakuganPlayAttackMotion` /
   `BtlBakuganSetStateFlag04` with states 7, 10 and 11). Returns at once when
   `BtlBakuganTryCancelIntoArt` switched to an art. Otherwise sets state flag 0x400000 and
   `artCancelTimer` 10, turns toward `attackObj` (`BtlBakuganTurnTowardTarget`; not with state
   flag 0x80000 past frame 7 of phase 1+, nor for kind 6 attack 0x3c past frame 28), clears
   `finalStepPlaying` and runs `attackPhase` (motion set = `attackMotions[attackIndex]`,
   `BtlAttackMotionSet`):
   - 0: flag 0x8000000 with set flag 2; at 90 % plays phase 2.
   - 2: flag 0x8000000 with set flag 2 before 30 % (or for kind 0x12); counts `attackMotionFlags`
     down once per `motionEnded`; then at 90 % (100 % for kind 13 with flag 0x80000) sets
     `finalStepPlaying`, clears flag 0x400000 and plays phase 3; when there is none, or the set
     lacks flag 1 and the unit is airborne, disarms `collider0`, clears flag 0x100, goes to state 0,
     runs the state-0 handler (model vtable slot 26), sets `attackPhase` 3 and returns.
   - 1, 3: flag 0x8000000 for kind 0x12 with set flag 2 before 30 %; at 90 % disarms the
     collider, goes to state 0, clears the 0xfffdf64f-masked attack command bits (also on the CPU
     unit's `BtlAi` pad when the "is CPU" virtual, slot 13, says so), runs the state-0 handler
     and returns; kind 0x14 in phase 3 of attack 0x3f clears flag 0x80000.
   Then (all other paths) `BtlBakuganSetStateFlag04`; on the ground (or with flag 0x80000)
   velocity.xyz *= 0.8 (w = 0) and `dashSpeed` *= 0.9, airborne vertical speed *= 0.84, x/z *= 0.93 and
   `dashSpeed` *= 0.93. With a motion set: flag 1 lifts attacks 0x3c..0x3f toward 110 above
   `groundY` (flag 0x1000000); flag 8/0x10 sets the motion speed (model vtable slot 6) to
   (frame/30 or frame/60, clamped to 0..1)^2; flag 0x4000 in phase 2 lerps the
   velocity by 0.3 toward the target gap (minus 1.3 x reach, clamped to 0..stat 4C) or forward at
   `BtlBakuganGetScaledStat4C`; flag 0x80 sets the velocity to `dashSpeed` toward the target
   (target at entry) or adds `dashSpeed` along the heading. Counts `attackFrame`, and
   `attackEventFrame` from phase 2.
   VFPU bank constants S703 (2/pi, folded into cosf/sinf of the yaw), S713 (0) and S733 (1) are
   literals. */

/* Virtual slot 6 (motion speed; `BtlBakuganSetMotionSpeed` for the base class). */
static inline void BtlBakuganCallSetMotionSpeed(BtlBakugan *self, float speed)
{
  const VtblEntry *entry = &((const VtblEntry *)self->base.base.vtable)[6];

  ((float (*)(float, void *))entry->fn)(speed, (u8 *)self + entry->delta);
}

/* Virtual slot 13 ("is CPU unit"). */
static inline s32 BtlBakuganCallIsCpuUnit(BtlBakugan *self)
{
  const VtblEntry *entry = &((const VtblEntry *)self->base.base.vtable)[13];

  return ((s32 (*)(void *))entry->fn)((u8 *)self + entry->delta);
}

/* Virtual slot 26 (state 0 handler; `BtlBakuganState00Update` for the base class). */
static inline void BtlBakuganCallState00Update(BtlBakugan *self)
{
  const VtblEntry *entry = &((const VtblEntry *)self->base.base.vtable)[26];

  ((void (*)(void *))entry->fn)((u8 *)self + entry->delta);
}

/* delta.xyz = to->pos - from->pos; delta.w = to->pos.w. */
static inline void BtlBakuganAimDelta(float *delta, BtlBakugan *to, BtlBakugan *from)
{
  float tw = to->base.pos[3];

  delta[0] = to->base.pos[0] - from->base.pos[0];
  delta[1] = to->base.pos[1] - from->base.pos[1];
  delta[2] = to->base.pos[2] - from->base.pos[2];
  delta[3] = tw;
}

/* aim.xyz *= len / |aim.xyz| (inverse length 0 for a zero vector); aim.w becomes 0. */
static inline void BtlBakuganScaleAim(float *aim, float len)
{
  float sq = aim[0] * aim[0] + aim[1] * aim[1] + aim[2] * aim[2];
  float inv = VfRsq(sq);
  float k;

  if (sq == 0.0f) {
    inv = 0.0f;
  }
  k = inv * len;
  aim[0] = aim[0] * k;
  aim[1] = aim[1] * k;
  aim[2] = aim[2] * k;
  aim[3] = 0.0f;
}

/* step = {cos(yaw), 0, sin(yaw)} * speed, step.w = 0. */
static inline void BtlBakuganHeadingStep(float *step, float yaw, float speed)
{
  step[0] = __builtin_cosf(yaw) * speed;
  step[1] = 0.0f * speed;
  step[2] = __builtin_sinf(yaw) * speed;
  step[3] = 0.0f;
}

void BtlBakuganState08Update(BtlBakugan *self)
{
  float aim[4];
  float dir[4];
  float step[4];
  const BtlAttackMotionSet *set;
  BtlBakugan *target;
  BtlAi *ai;
  float *velocity;
  float threshold;
  float rate;
  float vy;
  float dist;
  float gap;
  float speed;
  float yaw;
  int hadTarget;

  hadTarget = BtlBakuganGetTarget(self) != NULL;
  set = (const BtlAttackMotionSet *)self->attackMotions[self->attackIndex];
  if (BtlBakuganTryCancelIntoArt(self) != 0) {
    return;
  }
  self->stateFlags |= 0x400000;
  self->artCancelTimer = 10;
  if ((self->stateFlags & 0x80000) == 0 || (s32)self->attackPhase <= 0 ||
      self->attackFrame < 8) {
    if (!(self->base.base.unk08 == 6 && self->attackIndex == 0x3c && self->attackFrame >= 0x1d)) {
      BtlBakuganTurnTowardTarget(self, self->attackObj);
    }
  }
  self->finalStepPlaying = 0;

  switch ((s32)self->attackPhase) {
  case 0:
    if (set != NULL && (set->flags & 2) != 0) {
      self->stateFlags |= 0x8000000;
    }
    if (GfxModelMotionReached(&self->base, 0.9f)) {
      BtlBakuganPlayAttackMotion(self, 2);
    }
    break;

  case 2:
    if (set != NULL && (set->flags & 2) != 0) {
      if (!GfxModelMotionReached(&self->base, 0.3f) || self->base.base.unk08 == 0x12) {
        self->stateFlags |= 0x8000000;
      }
    }
    if ((s32)self->attackMotionFlags > 0) {
      if (self->base.motionEnded != 0) {
        self->base.motionEnded = 0;
        self->attackMotionFlags--;
      }
      break;
    }
    threshold = 0.9f;
    if (self->base.base.unk08 == 0xd && (self->stateFlags & 0x80000) != 0) {
      threshold = 1.0f;
    }
    if (GfxModelMotionReached(&self->base, threshold)) {
      self->finalStepPlaying = 1;
      self->stateFlags &= ~0x400000u;
      if (BtlBakuganPlayAttackMotion(self, 3) == 0 ||
          ((set->flags & 1) == 0 && BtlBakuganIsAirborne(self, 1) != 0)) {
        self->collider0->hitTimer = 0;
        self->collider0->flags &= ~1u;
        self->stateFlags &= ~0x100u;
        BtlBakuganSetState(self, 0, 0);
        BtlBakuganCallState00Update(self);
        self->attackPhase = 3;
        return;
      }
    }
    break;

  case 1:
  case 3:
    if (set != NULL && (set->flags & 2) != 0 && !GfxModelMotionReached(&self->base, 0.3f) &&
        self->base.base.unk08 == 0x12) {
      self->stateFlags |= 0x8000000;
    }
    if (GfxModelMotionReached(&self->base, 0.9f)) {
      self->collider0->hitTimer = 0;
      self->collider0->flags &= ~1u;
      self->stateFlags &= ~0x100u;
      BtlBakuganSetState(self, 0, 0);
      if (BtlBakuganCallIsCpuUnit(self) != 0) {
        self->commands &= 0xfffdf64f;
        ai = ((BtlCpuUnit *)self)->ai;
        if (ai != NULL) {
          ai->pad.cur.aiActions &= 0xfffdf64f;
        }
      }
      BtlBakuganCallState00Update(self);
      return;
    }
    if (self->base.base.unk08 == 0x14 && self->attackPhase == 3 && self->attackIndex == 0x3f) {
      self->stateFlags &= ~0x80000u;
    }
    break;

  default:
    break;
  }

  BtlBakuganSetStateFlag04(self);
  velocity = self->base.velocity;
  if (BtlBakuganIsAirborne(self, 1) == 0 || (self->stateFlags & 0x80000) != 0) {
    /* velocity.xyz *= 0.8; velocity.w becomes 0 */
    velocity[0] = velocity[0] * 0.8f;
    velocity[1] = velocity[1] * 0.8f;
    velocity[2] = velocity[2] * 0.8f;
    velocity[3] = 0.0f;
    self->dashSpeed = self->dashSpeed * 0.9f;
  } else {
    velocity[1] = velocity[1] * 0.84f;
    velocity[0] = velocity[0] * 0.93f;
    velocity[2] = velocity[2] * 0.93f;
    self->dashSpeed = self->dashSpeed * 0.93f;
  }

  if (set != NULL) {
    if ((set->flags & 1) != 0) {
      if (self->attackIndex >= 0x3c && self->attackIndex < 0x40) {
        vy = self->groundY + 110.0f - self->base.pos[1];
        if (vy < 0.0f) {
          vy = 0.0f;
        }
        velocity[1] = self->gravity + vy * 0.05f;
      }
      self->stateFlags |= 0x1000000;
    }
    rate = 0.0f;
    if ((set->flags & 8) != 0) {
      rate = 0.033333335f;
    }
    if ((set->flags & 0x10) != 0) {
      rate = 0.016666668f;
    }
    if (!(rate == 0.0f)) {
      rate = rate * (float)self->attackFrame;
      /* clamp to [0, 1] (vmin/vmax: rate is never NaN) */
      rate = rate < 1.0f ? rate : 1.0f;
      rate = rate > 0.0f ? rate : 0.0f;
      BtlBakuganCallSetMotionSpeed(self, rate * rate);
    }
    if ((set->flags & 0x4000) != 0 && self->attackPhase == 2) {
      target = (BtlBakugan *)BtlBakuganGetTarget(self);
      if (target != NULL) {
        BtlBakuganAimDelta(aim, target, self);
        dist = __builtin_sqrtf(aim[0] * aim[0] + aim[1] * aim[1] + aim[2] * aim[2]);
        gap = dist - self->combat.stats->reachRadius * 1.3f;
        speed = BtlBakuganGetScaledStat4C(self);
        if (gap < 0.0f) {
          gap = 0.0f;
        } else if (speed < gap) {
          gap = speed;
        }
        BtlBakuganScaleAim(aim, gap);
      } else {
        yaw = self->base.rot[1];
        speed = BtlBakuganGetScaledStat4C(self);
        BtlBakuganHeadingStep(aim, yaw, speed);
      }
      /* velocity += (aim - velocity) * 0.3 (all four lanes) */
      velocity[0] = velocity[0] + (aim[0] - velocity[0]) * 0.3f;
      velocity[1] = velocity[1] + (aim[1] - velocity[1]) * 0.3f;
      velocity[2] = velocity[2] + (aim[2] - velocity[2]) * 0.3f;
      velocity[3] = velocity[3] + (aim[3] - velocity[3]) * 0.3f;
      self->stateFlags |= 0x1000000;
    }
    if ((set->flags & 0x80) != 0) {
      if (hadTarget) {
        target = (BtlBakugan *)BtlBakuganGetTarget(self);
        BtlBakuganAimDelta(dir, target, self);
        BtlBakuganScaleAim(dir, self->dashSpeed);
        /* velocity = dir (all four lanes) */
        velocity[0] = dir[0];
        velocity[1] = dir[1];
        velocity[2] = dir[2];
        velocity[3] = dir[3];
      } else {
        BtlBakuganHeadingStep(step, self->base.rot[1], self->dashSpeed);
        /* velocity.xyz += step.xyz */
        velocity[0] = velocity[0] + step[0];
        velocity[1] = velocity[1] + step[1];
        velocity[2] = velocity[2] + step[2];
      }
    }
  }
  self->attackFrame++;
  if ((s32)self->attackPhase >= 2) {
    self->attackEventFrame++;
  }
}
