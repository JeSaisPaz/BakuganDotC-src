// bdc 0x0897a19c UiCollectionSpherePhaseLoad
#include "bdc.h"

/* Phase 1 of `UiCollectionSphere`: creates the sprites
   (`UiCollectionSphereCreateSprites`) and cameras (`UiCollectionSphereInitCameras`) in step 0
   and advances to the main phase on the next frame. */

void UiCollectionSpherePhaseLoad(UiCollectionSphere *self)

{
  if ((self->base).phaseStep == 0) {
    UiCollectionSphereCreateSprites(self);
    UiCollectionSphereInitCameras(self);
    (self->base).phaseStep = (self->base).phaseStep + 1;
  }
  else {
    (self->base).phaseStep = 0;
    (self->base).phase = (self->base).phase + 1;
  }
  return;
}

