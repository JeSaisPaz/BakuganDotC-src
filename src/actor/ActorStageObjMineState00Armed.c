// bdc 0x088a6298 ActorStageObjMineState00Armed
#include "bdc.h"

/* State 0 of the mine (table `0x08a83eb8`): while visible, triggers it
   (`ActorStageObjMineDetect`) by resetting the sequence step `+0x1fc` and switching to state 1.
    */

void ActorStageObjMineState00Armed(ActorStageObjMine *self)

{
  
  if (!(self->base.base.ambient[3] <= 0.0f) && ActorStageObjMineDetect(self) != 0) {
    (self->base).step = 0;
    self->state = 1;
  }
  return;
}

