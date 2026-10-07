// bdc 0x08a2c01c ActorStageObjIsTarget
#include "bdc.h"

/* Override of the stage-object class test slot 13 (`+0x6c`) by the script-created HP object
   (`ActorStageObjCtor`, vtable `0x08af2864`): returns 1. */

int ActorStageObjIsTarget(ActorStageObjBase *self)

{
  return 1;
}

