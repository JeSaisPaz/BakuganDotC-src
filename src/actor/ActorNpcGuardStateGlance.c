// bdc 0x088e91c4 ActorNpcGuardStateGlance
#include "bdc.h"

/* AI state 7 (slot 42) of the guard classes (`ActorNpcGuardCtor`, vtable `0x08af4024`; inherited
   by `ActorNpcCloakCtor`), overriding `ActorNpcState07Reset`: waits 70 frames (showing the `?`
   head effect 0x2a from frame 50, `ActorNpcShowHeadEffect`), turns toward the remembered point
   `noisePoint` (turning motions 9/10), idles 70 frames with the `?` again, then turns back to the
   home heading `homeHeading` and returns to state 0. It never walks. Does nothing when
   `ActorNpcCheckInterrupt` took over. */

void ActorNpcGuardStateGlance(ActorNpc *self)
{
  float turn;

  if (ActorNpcCheckInterrupt(self, 0) != 0) {
    return;
  }
  switch ((u32)self->subStep) {
  case 0:
    self->subStep = 1;
    self->timer = 70;
    ActorPlayMotion(0.2f, self, 0, 0, 0);
    break;
  case 1:
    if (self->timer == 0) {
      ActorNpcShowHeadEffect(self, 0x2a, 0, 1);
      self->subStep = 2;
    } else {
      self->timer = self->timer - 1;
      if (self->timer == 50) {
        ActorNpcShowHeadEffect(self, 0x2a, 1, 1);
      }
    }
    break;
  case 2:
    turn = ActorTurnToward(atan2f(self->noisePoint[2] - self->base.base.pos[2],
                                  self->noisePoint[0] - self->base.base.pos[0]),
                           1.0f, self->turnRate, self);
    if (turn * turn < 0.01f) {
      self->subStep = 3;
    } else if (turn < 0.0f) {
      ActorPlayMotion(0.2f, self, 10, 1, 0);
    } else {
      ActorPlayMotion(0.2f, self, 9, 1, 0);
    }
    break;
  case 3:
    ActorPlayMotion(0.2f, self, 0, 0, 0);
    self->subStep = 4;
    self->timer = 70;
    break;
  case 4:
    if (self->timer == 0) {
      ActorNpcShowHeadEffect(self, 0x2a, 0, 1);
      self->subStep = 5;
    } else {
      self->timer = self->timer - 1;
      if (self->timer == 50) {
        ActorNpcShowHeadEffect(self, 0x2a, 1, 1);
      }
    }
    break;
  case 5:
    turn = ActorTurnToward(self->homeHeading, 1.0f, self->turnRate, self);
    if (turn * turn < 0.01f) {
      self->aiState = 0;
      self->subStep = 0;
    } else if (turn < 0.0f) {
      ActorPlayMotion(0.2f, self, 10, 1, 0);
    } else {
      ActorPlayMotion(0.2f, self, 9, 1, 0);
    }
    break;
  }
}
