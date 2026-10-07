// bdc 0x088a3cb8 ActorStageObjEggCrystalSetState
#include "bdc.h"

/* Switches an egg crystal (`ActorStageObjEggCrystalCtor`) to state `state` (`+0x380`) and clears
   the step `+0x344` and step data `+0x348`. */

void ActorStageObjEggCrystalSetState(ActorStageObjEggCrystal *self, int state)

{
  self->step = 0;
  self->stepData = 0;
  self->state = state;
  return;
}

