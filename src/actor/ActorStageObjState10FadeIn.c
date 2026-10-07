// bdc 0x088aa204 ActorStageObjState10FadeIn
#include "bdc.h"

/* State 10 handler of the shared stage-object state machine (table `0x08a842f8`,
   `ActorStageObjUpdate`): fades the object in — step `+0x1fc` 0 zeroes the draw alpha `+0x6c`
   and the fade factor `+0x228`; step 1 raises `+0x228` by 0.1 per frame (fast-forward flag
   `0x08ac1b28` set) or 1/60 until it reaches 1.0; step 2 returns to state 0 (`+0x304`) and resets
   the step. */

void ActorStageObjState10FadeIn(ActorStageObjBase *self)

{
  int step;
  float delta;
  
  step = self->step;
  if (step < 1) {
    if (-1 < step) {
      (self->base).ambient[3] = 0.0f;
      self->fade = 0.0f;
      self->step = step + 1;
      return;
    }
  }
  else if (step < 2) {
    if (g_uiScreen390FastForward == 0) {
      delta = 0.016666668f;
    }
    else {
      delta = 0.1f;
    }
    delta = self->fade + delta;
    self->fade = delta;
    if (1.0f <= delta) {
      self->fade = 1.0f;
      self->step = step + 1;
      return;
    }
  }
  else if (step < 3) {
    self->state = 0;
    self->step = 0;
  }
  return;
}

