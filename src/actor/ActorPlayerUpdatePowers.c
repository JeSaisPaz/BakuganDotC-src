// bdc 0x088e1f30 ActorPlayerUpdatePowers
#include "bdc.h"

/* Updates the two special powers of the player actor class (model 0x2f `12_Edit_man.gmo`, 0x550
   bytes, constructor `ActorPlayerCtor`, vtable `0x08af38e4`) once they are unlocked by event
   flags 0x38e (power A: gauge `+0x3bc`, active flag `+0x3a0`, trigger command bit `0x8000000`) and
   0x38d (power B: gauge `+0x3c0`, active `+0x3a1`, trigger bit `0x10000000`). While a recharge is
   requested (`+0x3d0`) both gauges refill by 0.1 per frame and the loop sound 0x2c00038 plays; a
   trigger with a non-empty gauge activates the power (`ActorPlayerStealthOn` /
   `ActorPlayerScanOn`, unless task 500 is busy), an empty one plays the refusal sound 3, and an
   active power drains its gauge (0.0033 or 0.005 per frame) and switches off
   (`ActorPlayerStealthOff` / `ActorPlayerScanOff`) at 0. */

void ActorPlayerUpdatePowers(ActorPlayer *self)
{
  s32 state;
  float gauge;
  Actor *player;
  GameFieldTask *field;

  if (!GameEventFlagTest(0x38d) && !GameEventFlagTest(0x38e)) {
    return;
  }
  state = self->base.state;
  if (state != 0 && state != 1 && state != 6 && state != 7 && state != 9 && state != 0xb &&
      state != 0xc) {
    self->powerRequest = 1;
    self->powerCooldown = 0;
    self->stealth = 0;
    self->scan = 0;
    return;
  }

  /* Recharge zone: refill both gauges and keep the loop sound playing. */
  if (self->inChargeZone == 0) {
    if (SndHasManager()) {
      SndManagerStop(SndGetManager(), self->loopSound);
    }
    self->loopSoundPlaying = 0;
  } else {
    float a = self->gaugeA + 0.1f;
    float b = self->gaugeB + 0.1f;
    self->gaugeA = a;
    self->gaugeB = b;
    if (!(a <= 1.0f)) {
      self->gaugeA = 1.0f;
    }
    if (!(b <= 1.0f)) {
      self->gaugeB = 1.0f;
    }
    self->inChargeZone = 0;
    if (self->loopSoundPlaying == 0) {
      if (SndHasManager()) {
        self->loopSound = SndManagerPlay(SndGetManager(), 0x2c00038, 0, 0);
      }
      self->loopSoundPlaying = 1;
    }
  }

  /* Trigger: toggle power B, else power A. */
  if (GameEventFlagTest(0x38d) && (self->base.motion & 0x10000000) != 0) {
    player = (Actor *)ActorFindPlayer();
    if (player->state != 0xb) {
      if (self->scan != 0) {
        ActorPlayerScanOff(self, 0);
      } else if (self->gaugeB != 0.0f && self->base.detected == 0) {
        field = (GameFieldTask *)CoreTaskFind(500);
        if (field->catchActive == 0) {
          ActorPlayerScanOn(self);
        }
      }
    }
  } else if (GameEventFlagTest(0x38e) && (self->base.motion & 0x8000000) != 0) {
    player = (Actor *)ActorFindPlayer();
    if (player->state != 0xb) {
      if (self->stealth != 0) {
        ActorPlayerStealthOff(self, 0);
      } else if (self->gaugeA != 0.0f && self->base.detected == 0) {
        field = (GameFieldTask *)CoreTaskFind(500);
        if (field->catchActive == 0) {
          ActorPlayerStealthOn(self);
        }
      }
    }
  }

  /* Cooldown before the next trigger is accepted. */
  if (self->powerRequest == 0) {
    self->powerCooldown = self->powerCooldown + 1;
    if (!((float)self->powerCooldown <= 6.0f)) {
      self->powerRequest = 1;
      self->powerCooldown = 0;
    }
  }

  /* Trigger with an empty gauge: refusal sound. */
  if ((GameEventFlagTest(0x38e) && (self->base.motion & 0x8000000) != 0 &&
       self->gaugeA == 0.0f) ||
      (GameEventFlagTest(0x38d) && (self->base.motion & 0x10000000) != 0 &&
       self->gaugeB == 0.0f)) {
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 3, 0, 0);
    }
  }

  /* Drain the active power's gauge. */
  if (self->scan != 0 && self->powerRequest != 0) {
    if (ActorPlayerIsIdleOrThrowing(self)) {
      gauge = self->gaugeB - 0.0033333332f;
      self->gaugeB = gauge;
    } else {
      bool walking = ActorPlayerIsWalking(self);
      gauge = self->gaugeB;
      if (walking) {
        gauge = gauge - 0.005f;
        self->gaugeB = gauge;
      }
    }
    if (gauge < 0.0f) {
      self->gaugeB = 0.0f;
      ActorPlayerScanOff(self, 0);
    }
  }
  if (self->stealth != 0 && self->powerRequest != 0) {
    if (ActorPlayerIsIdleOrThrowing(self)) {
      gauge = self->gaugeA - 0.0033333332f;
      self->gaugeA = gauge;
    } else {
      bool walking = ActorPlayerIsWalking(self);
      gauge = self->gaugeA;
      if (walking) {
        gauge = gauge - 0.005f;
        self->gaugeA = gauge;
      }
    }
    if (gauge < 0.0f) {
      self->gaugeA = 0.0f;
      ActorPlayerStealthOff(self, 0);
    }
  }
}
