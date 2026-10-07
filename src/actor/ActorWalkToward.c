// bdc 0x088e0784 ActorWalkToward
#include "bdc.h"

/* Steers an actor toward `target` for one frame (vtable slot 18, `+0x94`): returns 0 at once while
   the player is free to move (GameFieldIsPlayerFree). Otherwise turns the heading toward the target
   (`atan2f` of the XZ delta, `ActorTurnToward(angle, 1.0, 0.139)`); while still turning (remaining
   turn^2 not < 0.01) the velocity is zeroed and models 0x4e..0x50 play turn motion 9/10; once
   facing it, returns 1 if the XZ distance is < `arriveDist`, else sets `velocity` to the heading's
   forward vector (cos, 0, sin, 0) times `speed`. When not arrived, the stall check against
   `checkpoint` runs: moving less than `speed` from it for more than 30 frames returns 1, else 0. */

int ActorWalkToward(Actor *self, float *target, float speed, float arriveDist)
{
  int done;
  int facing;
  float angle;
  float diff;
  float dx;
  float dy;
  float dz;
  float dist;
  float dist2;
  s32 slot;

  done = 0;
  if (GameFieldIsPlayerFree(GameFieldFindTask())) {
    return 0;
  }
  angle = atan2f(target[2] - self->base.pos[2], target[0] - self->base.pos[0]);
  facing = 0;
  diff = ActorTurnToward(angle, 1.0f, 0.13962634f, self);
  if (diff * diff < 0.01f) {
    facing = 1;
  }
  if (facing) {
    /* XZ distance: both points with y forced to 0 (vsub.q / vdot.t / vsqrt.s) */
    dx = self->base.pos[0] - target[0];
    dz = self->base.pos[2] - target[2];
    dist = __builtin_sqrtf(dx * dx + dz * dz);
    if (dist < arriveDist) {
      done = 1;
    } else {
      /* vrot.q [C,0,S,0] of rot[1] * 2/pi, then vscl.t by speed (w stays 0) */
      angle = self->base.rot[1];
      self->base.velocity[0] = __builtin_cosf(angle) * speed;
      self->base.velocity[1] = 0.0f * speed;
      self->base.velocity[2] = __builtin_sinf(angle) * speed;
      self->base.velocity[3] = 0.0f;
    }
  } else {
    /* sv.q C720: bank zero vector */
    self->base.velocity[0] = 0.0f;
    self->base.velocity[1] = 0.0f;
    self->base.velocity[2] = 0.0f;
    self->base.velocity[3] = 0.0f;
    if ((s32)self->base.base.unk08 >= 0x4e && (s32)self->base.base.unk08 < 0x51) {
      slot = 9;
      if (diff < 0.0f) {
        slot = 10;
      }
      ActorPlayMotion(0.2f, self, slot, 0, 0);
    }
  }
  if (!done) {
    dx = self->checkpoint[0] - self->base.pos[0];
    dy = self->checkpoint[1] - self->base.pos[1];
    dz = self->checkpoint[2] - self->base.pos[2];
    dist2 = dx * dx + dy * dy + dz * dz;
    if (dist2 < speed * speed) {
      self->stuckFrames++;
      if (self->stuckFrames >= 0x1f) {
        self->stuckFrames = 0;
        done = 1;
      }
    } else {
      self->checkpoint[0] = self->base.pos[0];
      self->checkpoint[1] = self->base.pos[1];
      self->checkpoint[2] = self->base.pos[2];
      self->checkpoint[3] = self->base.pos[3];
      self->stuckFrames = 0;
    }
  }
  return done;
}
