// bdc 0x088b11e8 ActorStageObjPropState01Idle
#include "bdc.h"

/* State 1 of the knock-over prop: on contact (`ActorStageObjPropDetectContact`) resets the
   sub-step `+0x328` and switches to state 2 (knocked flying), or 3 (topple) for signs (category 7).
    */

void ActorStageObjPropState01Idle(ActorStageObjProp *self)

{
  if (ActorStageObjPropDetectContact(self) != 0) {
    self->step = 0;
    self->state = ((self->base).category == 7) ? 3 : 2;
  }
  return;
}

