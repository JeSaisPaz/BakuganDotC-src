// bdc 0x088e8e3c ActorNpcGuardStateWait
#include "bdc.h"

/* AI state 1 of the guard classes (slot 36): unless interrupted (`ActorNpcCheckInterrupt`),
   counts `timer` down; once it reaches 0 returns to state 0 (subStep 0) and advances the route
   step. Then plays motion slot 0 without looping. */

void ActorNpcGuardStateWait(ActorNpc *self)
{
  s32 step;

  if (ActorNpcCheckInterrupt(self, 0) != 0) {
    return;
  }
  if (self->timer > 0) {
    self->timer = self->timer - 1;
  }
  else {
    step = self->base.routeStep;
    self->aiState = 0;
    self->subStep = 0;
    self->base.routeStep = step + 1;
  }
  ActorPlayMotion(0.2f, self, 0, 0, 0);
}
