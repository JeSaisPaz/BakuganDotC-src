// bdc 0x08a2bfb0 ActorStageObjBaseIsAttrLandmark
#include "bdc.h"

/* Base stage-object class test, vtable slot 12 (`+0x64`): returns 0 (not an attribute landmark);
   overridden by `ActorStageObjAttrLandmarkIsAttrLandmark`. */

int ActorStageObjBaseIsAttrLandmark(ActorStageObjBase *self)

{
  return 0;
}

