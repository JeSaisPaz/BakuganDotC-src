// bdc 0x088a2188 ActorStageObjLandmarkSetState
#include "bdc.h"

/* Switches the landmark to state `state` (`+0x3ec`) and clears the state's step counter `+0x3e4`
   and timer `+0x3e8`. */

void ActorStageObjLandmarkSetState(ActorStageObjLandmark *self, int state)

{
  self->step = 0;
  self->timer = 0;
  self->state = state;
  return;
}

