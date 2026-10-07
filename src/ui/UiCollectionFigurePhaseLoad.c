// bdc 0x0898c170 UiCollectionFigurePhaseLoad
#include "bdc.h"

/* Phase 1 of `UiCollectionFigure`: creates the sprites
   (`UiCollectionFigureCreateSprites`) and cell cameras (`UiCollectionFigureInitCameras`) in
   step 0 and advances to the main phase on the next frame. */

void UiCollectionFigurePhaseLoad(UiCollectionFigure *self)

{
  if ((self->base).phaseStep == 0) {
    UiCollectionFigureCreateSprites(self);
    UiCollectionFigureInitCameras(self);
    (self->base).phaseStep = (self->base).phaseStep + 1;
  }
  else {
    (self->base).phaseStep = 0;
    (self->base).phase = (self->base).phase + 1;
  }
  return;
}

