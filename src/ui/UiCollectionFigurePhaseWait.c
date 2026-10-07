// bdc 0x0898bdc4 UiCollectionFigurePhaseWait
#include "bdc.h"

/* First phase (phase list at slot `0x08a9e9f4` of table `0x08a9e9e8`) of the UiCollectionFigure
   collection screen: waits one frame (`+0x2c`), then advances the phase `+0x28` to
   `UiCollectionFigurePhaseLoad`. */

void UiCollectionFigurePhaseWait(UiCollectionFigure *self)

{
  if ((self->base).phaseStep == 0) {
    (self->base).phaseStep = 1;
    return;
  }
  (self->base).phaseStep = 0;
  (self->base).phase = (self->base).phase + 1;
  return;
}

