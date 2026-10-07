// bdc 0x0898c1c8 UiCollectionFigurePhaseClose
#include "bdc.h"

/* Last phase (slot `0x08a9ea0c` of table `0x08a9e9e8`) of the UiCollectionFigure collection screen:
   requests the screen's close (`closeRequested`, `+0x4c` = 1). */

void UiCollectionFigurePhaseClose(UiCollectionFigure *self)

{
  (self->base).closeRequested = '\x01';
  return;
}

