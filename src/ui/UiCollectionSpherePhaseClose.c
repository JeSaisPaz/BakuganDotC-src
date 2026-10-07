// bdc 0x0897a1f4 UiCollectionSpherePhaseClose
#include "bdc.h"

/* Last phase (slot `0x08a9dca4` of table `0x08a9dc38`) of the UiCollectionSphere collection screen:
   requests the screen's close (`closeRequested`, `+0x4c` = 1). */

void UiCollectionSpherePhaseClose(UiCollectionSphere *self)

{
  (self->base).closeRequested = '\x01';
  return;
}

