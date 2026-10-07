// bdc 0x08a2bffc ActorStageObjEggCrystalIsEggCrystal
#include "bdc.h"

/* Egg-crystal override of the stage-object class test slot 14 (`+0x74`): returns 1 (shared by the
   egg crystal `0x08af2524` and egg crystal element `0x08af25c4` vtables). */

int ActorStageObjEggCrystalIsEggCrystal(ActorStageObjEggCrystal *self)

{
  return 1;
}

