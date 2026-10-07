// bdc 0x088e31cc ActorPlayerStateCaught
#include "bdc.h"

/* State 9 (caught) handler of the player actor class (model 0x2f `12_Edit_man.gmo`, 0x550 bytes,
   constructor `ActorPlayerCtor`, vtable `0x08af38e4`) (non-virtual entry of the state table
   `0x08a98ae0`), stepped by `waitTimer` with the countdown `stateDelay`:
   - 0: plays the collapse motion 0x1b (end frame 16), sets the fader to red (start/colour
     (1,0,0,0.25), end (1,0,0,0.5), preset 3, 30 frames, enabled) -> step 1, delay 45;
   - 1: when the motion has finished, loops motion 0 -> step 0x457, delay 30;
   - 0x457: after the delay calls `ActorPlayerCancelPowers`; with the save profile's
     `fieldCounter > 0` fades from the current colour to alpha 0 (preset 3, 30 frames) -> step 3,
     otherwise -> step 2 (delay 45 either way);
   - 2: after the delay, once the fade has finished sets `alerted` -> step 3;
   - 3: after the delay, once the fade has finished sets `alerted` and clears the field task's
     (task 500) `catchActive`.
   In steps 1, 2 and 0x457 (unless it just moved to step 3), a finished fade is restarted with
   preset 4 over 30 frames. Every frame the player turns toward the point `caughtBy`. */

void ActorPlayerStateCaught(ActorPlayer *self)
{
  bool refade;
  s32 step;
  GfxFader *dst;
  GfxFader *src;
  GfxFader *fader;
  GameFieldTask *task;
  float angle;
  const float *target;

  step = self->base.waitTimer;
  refade = false;
  if (step == 0x457) {
    refade = true;
    if (self->base.stateDelay > 0) {
      self->base.stateDelay = self->base.stateDelay - 1;
    } else {
      ActorPlayerCancelPowers(self);
      if (SaveGetProfile()->data->fieldCounter > 0) {
        self->base.waitTimer = 3;
        self->base.stateDelay = 0x2d;
        dst = GfxGetActiveFader();
        src = GfxGetActiveFader();
        /* end = colour */
        dst->end[0] = src->color[0];
        dst->end[1] = src->color[1];
        dst->end[2] = src->color[2];
        dst->end[3] = src->color[3];
        fader = GfxGetActiveFader();
        fader->end[3] = 0.0f;
        GfxFaderSetPreset(GfxGetActiveFader(), 3);
        GfxFaderStart(GfxGetActiveFader(), 0x1e);
        refade = false;
      } else {
        self->base.waitTimer = 2;
        self->base.stateDelay = 0x2d;
      }
    }
  } else if (step == 3) {
    refade = false;
    if (self->base.stateDelay > 0) {
      self->base.stateDelay = self->base.stateDelay - 1;
    } else if (GfxFaderIsFinished(GfxGetActiveFader())) {
      self->base.alerted = 1;
      task = CoreTaskFind(500);
      if (task->catchActive != 0) {
        task = CoreTaskFind(500);
        task->catchActive = 0;
      }
    }
  } else if (step == 2) {
    refade = true;
    if (self->base.stateDelay > 0) {
      self->base.stateDelay = self->base.stateDelay - 1;
    } else if (GfxFaderIsFinished(GfxGetActiveFader())) {
      self->base.alerted = 1;
      self->base.waitTimer = 3;
      refade = false;
    }
  } else if (step == 1) {
    if (GfxModelGetMotionRemaining(&self->base.base) == 0.0f) {
      ActorPlayMotion(0.2f, self, 0, 1, 0);
      self->base.waitTimer = 0x457;
      self->base.stateDelay = 0x1e;
    }
    refade = true;
  } else if (step == 0) {
    ActorPlayMotion(0.2f, self, 0x1b, 0, 0);
    GfxModelSetMotionEnd(&self->base.base, 16.0f);
    fader = GfxGetActiveFader();
    fader->start[0] = 1.0f;
    fader->start[1] = 0.0f;
    fader->start[2] = 0.0f;
    fader->start[3] = 0.25f;
    dst = GfxGetActiveFader();
    src = GfxGetActiveFader();
    /* colour = start */
    dst->color[0] = src->start[0];
    dst->color[1] = src->start[1];
    dst->color[2] = src->start[2];
    dst->color[3] = src->start[3];
    fader = GfxGetActiveFader();
    fader->end[0] = 1.0f;
    fader->end[1] = 0.0f;
    fader->end[2] = 0.0f;
    fader->end[3] = 0.5f;
    GfxFaderSetPreset(GfxGetActiveFader(), 3);
    GfxFaderStart(GfxGetActiveFader(), 0x1e);
    GfxFaderSetEnabled(GfxGetActiveFader(), 1);
    self->base.waitTimer = 1;
    self->base.stateDelay = 0x2d;
  }

  if (refade && GfxFaderIsFinished(GfxGetActiveFader())) {
    GfxFaderSetPreset(GfxGetActiveFader(), 4);
    GfxFaderStart(GfxGetActiveFader(), 0x1e);
  }
  if (self->caughtBy != NULL) {
    target = (const float *)self->caughtBy;
    angle = atan2f(target[2] - self->base.base.pos[2], target[0] - self->base.base.pos[0]);
    ActorTurnToward(angle, 1.0f, 0.13962634f, self);
  }
}
