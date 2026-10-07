// bdc 0x088e8bd0 ActorNpcRobotStateWait
#include "bdc.h"

/* AI state 1 of the robot classes (slot 36): counts down `+0x3b0`, then returns to state 0 and
   advances the route step; keeps the idle motion. */

void ActorNpcRobotStateWait(ActorNpc *self)

{
  if (self->timer < 1) {
    s32 step = (self->base).routeStep;
    self->aiState = 0;
    self->subStep = 0;
    (self->base).routeStep = step + 1;
  }
  else {
    self->timer = self->timer + -1;
  }
  ActorPlayMotion(0.2,self,0,'\x01','\0');
  return;
}

