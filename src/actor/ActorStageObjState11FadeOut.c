// bdc 0x088aa2b4 ActorStageObjState11FadeOut
#include "bdc.h"

/* State 11 handler of the shared stage-object state machine (table `0x08a842f8`,
   `ActorStageObjUpdate`): fades the object out — step `+0x1fc` 0 sets the fade factor `+0x228`
   to 1.0f; step 1 lowers it by 0.1 per frame (fast-forward flag ``g_uiScreen390FastForward`` set) or 1/60 until it
   reaches 0; step 2 sets the removal request `+0x282`. */

void ActorStageObjState11FadeOut(ActorStageObjBase *self)

{
  int step;
  float delta;
  
  step = self->step;
  if (step < 1) {
    if (-1 < step) {
      self->fade = 1.0f;
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
    delta = self->fade - delta;
    self->fade = delta;
    if (delta <= 0.0f) {
      self->fade = 0.0f;
      self->step = step + 1;
      return;
    }
  }
  else if (step < 3) {
    self->removeRequest = 1;
  }
  return;
}

