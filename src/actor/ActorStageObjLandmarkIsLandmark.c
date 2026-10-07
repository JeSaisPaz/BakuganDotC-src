// bdc 0x08a2bfe4 ActorStageObjLandmarkIsLandmark
#include "bdc.h"

/* Landmark override of the stage-object class test slot 17 (`+0x8c`): returns 1. */

int ActorStageObjLandmarkIsLandmark(ActorStageObjLandmark *self)

{
  return 1;
}

