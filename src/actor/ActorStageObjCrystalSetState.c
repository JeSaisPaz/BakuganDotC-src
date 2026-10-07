// bdc 0x088b4af4 ActorStageObjCrystalSetState
#include "bdc.h"

/* Switches the crystal stage object to state `state` (`+0x390`, see `ActorStageObjCrystalUpdate`)
   and clears its step `+0x328`. */

void ActorStageObjCrystalSetState(ActorStageObjCrystal *self, int state)

{
  self->step = 0;
  self->state = state;
  return;
}

