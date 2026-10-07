// bdc 0x088e74c4 ActorNpcStateLookAround
#include "bdc.h"

/* AI state 3 of the field NPC/guard classes (base `ActorNpcCtor`) (slot 38): unless
   `ActorNpcCheckInterrupt` took over, waits 45 frames (showing the `?` effect 0x2a from frame 30),
   then turns toward the heading `lookHeading` with turning motions 9/10 (not for the 0x51..0x53
   robot models); once the remaining turn is under 0.1 rad, hides the effect, returns to state 0 and
   advances the route step. */

void ActorNpcStateLookAround(ActorNpc *self)
{
  s32 step;
  s32 t;
  s32 model;
  s32 aligned;
  float diff;

  if (ActorNpcCheckInterrupt(self, 0) != 0) {
    return;
  }
  step = self->subStep;
  aligned = 0;
  if (step <= 0) {
    if (step < 0) {
      return;
    }
    self->timer = 45;
    self->subStep = step + 1;
  } else if (step == 1) {
    t = self->timer;
    if (t <= 0) {
      ActorNpcShowHeadEffect(self, 0x2a, 0, 1);
      self->subStep = 2;
    } else {
      self->timer = t - 1;
      if (t - 1 == 30) {
        ActorNpcShowHeadEffect(self, 0x2a, 1, 1);
      }
    }
  } else if (step == 2) {
    diff = ActorTurnToward(self->lookHeading, 1.0f, self->turnRate, self);
    if (diff * diff < 0.01f) {
      aligned = 1;
    }
    model = (s32)self->base.base.base.unk08;
    if (model < 0x51 || model >= 0x54) {
      if (diff < 0.0f) {
        ActorPlayMotion(0.2f, self, 10, 1, 0);
      } else {
        ActorPlayMotion(0.2f, self, 9, 1, 0);
      }
    }
    if (aligned) {
      self->aiState = 0;
      self->subStep = 0;
      self->base.routeStep++;
      ActorNpcShowHeadEffect(self, 0x2a, 0, 1);
    }
  }
}
