// bdc 0x08a2bfc8 ActorStageObjBaseIsCrystal
#include "bdc.h"

/* Base stage-object class test, vtable slot 15 (`+0x7c`): returns 0 (not a crystal stage object);
   overridden by `ActorStageObjCrystalIsCrystal`. */

int ActorStageObjBaseIsCrystal(ActorStageObjBase *self)

{
  return 0;
}

