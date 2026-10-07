// bdc 0x08a2bfc0 ActorStageObjBaseIsEggCrystal
#include "bdc.h"

/* Base stage-object class test, vtable slot 14 (`+0x74`): returns 0 (not an egg crystal);
   overridden by `ActorStageObjEggCrystalIsEggCrystal`. */

int ActorStageObjBaseIsEggCrystal(ActorStageObjBase *self)

{
  return 0;
}

