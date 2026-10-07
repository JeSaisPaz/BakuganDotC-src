// bdc 0x088e69c4 ActorNpcWalkToward
#include "bdc.h"

/* NPC version (vtable slot 18, `+0x94`) of `ActorWalkToward` for the field NPC/guard actor
   classes (models 0x4e..0x53). Returns 0 at once while the player is free to move
   (GameFieldIsPlayerFree). Otherwise turns toward `target` (`atan2f` of the XZ delta,
   `ActorTurnToward(angle, 1.0, turnRate)`); while still turning (remaining turn^2 not < 0.01) the
   velocity is zeroed and models 0x4e..0x50 play turn motion 9/10. Once facing it (heading
   snapped to the angle): if the XZ distance is < `arriveDist`, plays motion 0, stops the footstep
   loop 0x2c0003e, optionally (`snap`) sets the velocity to the remaining distance along the heading,
   and returns 1. Else plays the walk motion (models 0x4e..0x50: g_npcWalkFootsteps[run] slot with
   random footsteps 0x2c00001..6 on its step frames; 0x51..0x53: slot 1 plus the 0x2c0003e loop) and
   sets the velocity to the heading's forward vector times `speed`. When not arrived, the stall check
   against `checkpoint` runs: within `speed` of it and of the target returns 1 (checkpoint = target),
   else 0 (checkpoint = position when moved further than `speed`). */

int ActorNpcWalkToward(ActorNpc *self, float *target, float speed, float arriveDist, s32 run, s8 snap)
{
  float *pos;
  float *vel;
  float *chk;
  int done;
  int facing;
  float angle;
  float diff;
  float dx, dy, dz;
  float dist;
  float dist2;
  float speed2;
  float heading;
  s32 slot;
  s32 *gait;
  u32 rnd;

  done = 0;
  if (GameFieldIsPlayerFree(GameFieldFindTask())) {
    return 0;
  }
  pos = self->base.base.pos;
  vel = self->base.base.velocity;
  chk = self->base.checkpoint;
  angle = atan2f(target[2] - pos[2], target[0] - pos[0]);
  facing = 0;
  diff = ActorTurnToward(angle, 1.0f, self->turnRate, self);
  if (diff * diff < 0.01f) {
    self->base.base.rot[1] = angle;
    facing = 1;
  }
  if (facing) {
    /* XZ distance: both points with y = 0 */
    dx = pos[0] - target[0];
    dy = 0.0f - 0.0f;
    dz = pos[2] - target[2];
    dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    if (dist < arriveDist) {
      ActorPlayMotion(0.2f, self, 0, 1, 0);
      SndObjectStopSound(self->base.base.sound, 0x2c0003e);
      done = 1;
      if (snap) {
        heading = self->base.base.rot[1];
        vel[0] = __builtin_cosf(heading) * dist;
        vel[1] = 0.0f * dist;
        vel[2] = __builtin_sinf(heading) * dist;
        vel[3] = 0.0f;
      }
    } else {
      slot = 1;
      gait = NULL;
      if ((u32)(self->base.base.base.unk08 - 0x4e) < 6) {
        if (self->base.base.base.unk08 - 0x4e == 0 || self->base.base.base.unk08 - 0x4e == 1 ||
            self->base.base.base.unk08 - 0x4e == 2) {
          gait = g_npcWalkFootsteps[run];
          slot = gait[0];
        } else {
          SndObjectAddEmitter(self->base.base.sound, 0x2c0003e, 1, 1);
        }
      }
      if (ActorPlayMotion(0.2f, self, slot, 1, 0) != 0) {
        self->base.stepCounter = 0;
      } else if (gait != NULL) {
        self->base.stepCounter++;
        if (gait[1] < self->base.stepCounter) {
          self->base.stepCounter = 0;
        }
        if (self->base.stepCounter == gait[2] || self->base.stepCounter == gait[3]) {
          rnd = PlatformRandU32();
          SndObjectAddEmitter(self->base.base.sound, (((rnd >> 16) * 6) >> 16) + 0x2c00001, 0, 0);
        }
      }
      heading = self->base.base.rot[1];
      vel[0] = __builtin_cosf(heading) * speed;
      vel[1] = 0.0f * speed;
      vel[2] = __builtin_sinf(heading) * speed;
      vel[3] = 0.0f;
    }
  } else {
    vel[0] = 0.0f;
    vel[1] = 0.0f;
    vel[2] = 0.0f;
    vel[3] = 0.0f;
    if ((s32)self->base.base.base.unk08 >= 0x4e && (s32)self->base.base.base.unk08 < 0x51) {
      slot = 9;
      if (diff < 0.0f) {
        slot = 10;
      }
      ActorPlayMotion(0.2f, self, slot, 1, 0);
    }
  }
  if (!done) {
    /* 3D squared distance from the checkpoint */
    dx = chk[0] - pos[0];
    dy = chk[1] - pos[1];
    dz = chk[2] - pos[2];
    dist2 = dx * dx + dy * dy + dz * dz;
    speed2 = speed * speed;
    if (dist2 < speed2) {
      /* XZ squared distance to the target */
      dx = target[0] - pos[0];
      dy = 0.0f;
      dz = target[2] - pos[2];
      dist2 = dx * dx + dy * dy + dz * dz;
      if (dist2 < speed2) {
        chk[0] = target[0];
        chk[1] = target[1];
        chk[2] = target[2];
        chk[3] = target[3];
        self->base.stuckFrames = 0;
        done = 1;
      }
    } else {
      chk[0] = pos[0];
      chk[1] = pos[1];
      chk[2] = pos[2];
      chk[3] = pos[3];
      self->base.stuckFrames = 0;
    }
  }
  return done;
}
