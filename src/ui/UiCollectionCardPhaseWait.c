// bdc 0x08982b54 UiCollectionCardPhaseWait
#include "bdc.h"

/* First phase (phase list at slot `0x08a9e83c` of table `0x08a9e7e0`) of the UiCollectionCard
   collection screen: waits one frame (`+0x2c`), then advances the phase `+0x28` to
   `UiCollectionCardPhaseLoad`. */

void UiCollectionCardPhaseWait(UiCollectionCard *self)

{
  if ((self->base).phaseStep == 0) {
    (self->base).phaseStep = 1;
    return;
  }
  (self->base).phaseStep = 0;
  (self->base).phase = (self->base).phase + 1;
  return;
}

