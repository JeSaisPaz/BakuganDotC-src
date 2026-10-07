// bdc 0x088a7600 ActorStageObjAttrLandmarkSetIdle
#include "bdc.h"

/* Sets the companion's `companionMode` to 2 (idle) when there is a companion, and the landmark's
   `mode` to 1. Called by `ActorStageObjState00Update`. */

void ActorStageObjAttrLandmarkSetIdle(ActorStageObjAttrLandmark *self)
{
  if (self->companion != (ActorStageObjBase *)0x0) {
    self->companion->companionMode = 2;
  }
  self->mode = 1;
}
