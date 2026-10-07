// bdc 0x08a2bfec ActorStageObjBaseIsLandmark
#include "bdc.h"

/* Base stage-object class test, vtable slot 17 (`+0x8c`): returns 0; only the landmark overrides it
   (`ActorStageObjLandmarkIsLandmark`). */

int ActorStageObjBaseIsLandmark(ActorStageObjBase *self)

{
  return 0;
}

