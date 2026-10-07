// bdc 0x08a2bfb8 ActorStageObjBaseIsTarget
#include "bdc.h"

/* Base stage-object class test, vtable slot 13 (`+0x6c`): returns 0 (not a script-created HP
   target); overridden by `ActorStageObjIsTarget`. */

int ActorStageObjBaseIsTarget(ActorStageObjBase *self)

{
  return 0;
}

