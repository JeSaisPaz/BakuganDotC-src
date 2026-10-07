// bdc 0x08979d54 UiCollectionSpherePhaseWait
#include "bdc.h"

/* First phase (phase list at slot `0x08a9dc8c` of table `0x08a9dc38`) of the UiCollectionSphere
   collection screen: waits one frame (`+0x2c`), then advances the phase `+0x28` to
   `UiCollectionSpherePhaseLoad`. */

void UiCollectionSpherePhaseWait(UiCollectionSphere *self)

{
  if ((self->base).phaseStep == 0) {
    (self->base).phaseStep = 1;
    return;
  }
  (self->base).phaseStep = 0;
  (self->base).phase = (self->base).phase + 1;
  return;
}

