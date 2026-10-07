// bdc 0x088e7710 ActorNpcStateCatchPlayer
#include "bdc.h"

/* AI state 4 (vtable slot 39, `+0x13c`) of the field NPC/guard actor classes (models 0x4e..0x53,
   base `ActorNpcCtor`): step 0 catches the player at once: pauses the character set
   (`g_gameFieldCharSet` `paused`), shows head effect 0x2a for 30 frames
   (`ActorNpcShowHeadEffect`), puts the player in state 9 (`ActorPlayerStateCaught`) facing the
   NPC (`caughtBy`, `behaviourMark` = the NPC's placement slot), marks both as `detected`, and sets
   the field's talk blend rate (for exempt guards `ActorNpcIsChaseExempt` also forces the talk
   blend and interpolates the rate by distance); step 1 runs to the player (slot 18, speed 2 x
   `speed`) or, when the field forces the talk blend, only turns toward it, and stops (motion 0,
   sound 0x2c0003e off) while the field is in sub-states 0x13..0x16; step 2, once the head effect
   timer has run out, sets `alerted` and takes the penalty once (`ActorPlayerTakePenalty`: 300
   points) unless event counter `g_gameEventFlags[5]` is set. Does nothing when
   `ActorNpcCheckInterrupt` took over. */

void ActorNpcStateCatchPlayer(ActorNpc *self)
{
  ActorPlayer *player;
  GameFieldTask *task;
  const VtblEntry *entry;
  s32 counting;
  s32 step;
  u8 stop;
  float target[4];
  float blend;
  float d[3];
  float distSq;
  float turn;

  if (ActorNpcCheckInterrupt(self, 1) != 0) {
    return;
  }
  counting = 0;
  if (self->subStep > 0 && self->timer > 0) {
    counting = 1;
    self->timer = self->timer - 1;
    if (self->timer == 0) {
      ActorNpcShowHeadEffect(self, 0x2a, 0, 1);
    }
  }
  step = self->subStep;
  if (step <= 0) {
    if (step < 0) {
      return;
    }
    g_gameFieldCharSet->paused = 1;
    ActorNpcShowHeadEffect(self, 0x2a, 1, 1);
    self->timer = 30;
    self->subStep = self->subStep + 1;
    player = ActorFindPlayer();
    if (player != NULL) {
      if (player->base.state != 9) {
        ActorSetState(&player->base, 9, 0);
      }
      self->base.detected = 1;
      if (player->base.detected == 0) {
        player->base.detected = 1;
        player->behaviourMark = self->base.placementSlot;
        player->caughtBy = self->base.base.pos;
      }
    }
    blend = g_npcCatchBlendMin;
    if (self->base.placement != NULL && ActorNpcIsChaseExempt(self) != 0) {
      ((GameFieldTask *)GameFieldFindTask())->talkBlendForce = 1;
      /* squared distance NPC-player (xyz) */
      d[0] = self->base.base.pos[0] - player->base.base.pos[0];
      d[1] = self->base.base.pos[1] - player->base.base.pos[1];
      d[2] = self->base.base.pos[2] - player->base.base.pos[2];
      distSq = d[0] * d[0] + d[1] * d[1] + d[2] * d[2];
      blend = (distSq / (self->viewDist * self->viewDist)) *
                  (g_npcCatchBlendMax - g_npcCatchBlendMin) +
              g_npcCatchBlendMin;
      if (g_npcCatchBlendMax < blend) {
        blend = g_npcCatchBlendMax;
      }
    }
    ((GameFieldTask *)GameFieldFindTask())->talkBlendRate = blend;
  } else if (step < 2) {
    player = ActorFindPlayer();
    if (player == NULL) {
      return;
    }
    task = (GameFieldTask *)GameFieldFindTask();
    stop = 0;
    if ((s32)task->subState >= 0x13 && (s32)task->subState < 0x17) {
      stop = 1;
    }
    if (stop) {
      ActorPlayMotion(0.2f, self, 0, 1, 0);
      SndObjectStopSound(self->base.base.sound, 0x2c0003e);
      self->subStep = self->subStep + 1;
      return;
    }
    if (((GameFieldTask *)GameFieldFindTask())->talkBlendForce != 0) {
      target[0] = player->base.base.pos[0];
      target[2] = player->base.base.pos[2];
      turn = atan2f(target[2] - self->base.base.pos[2], target[0] - self->base.base.pos[0]);
      turn = ActorTurnToward(turn, 1.0f, self->turnRate, self);
      if (!(turn * turn < 0.01f)) {
        return;
      }
      ActorPlayMotion(0.2f, self, 0, 1, 0);
      self->subStep = self->subStep + 1;
    } else {
      entry = (const VtblEntry *)self->base.base.base.vtable + 18;
      if (((s32 (*)(void *, float *, s32, s32, float, float))entry->fn)(
              (char *)self + entry->delta, player->base.base.pos, 1, 0, self->speed * 2.0f,
              15.0f) != 0) {
        self->subStep = self->subStep + 1;
      }
    }
  } else if (step < 3) {
    if (counting) {
      return;
    }
    self->base.alerted = 1;
    player = ActorFindPlayer();
    if (player == NULL || player->base.alerted != 0) {
      return;
    }
    player = ActorFindPlayer();
    if (player == NULL) {
      return;
    }
    player->base.alerted = 1;
    if (g_gameEventFlags[5] != 0) {
      return;
    }
    ActorPlayerTakePenalty(player);
  }
}
