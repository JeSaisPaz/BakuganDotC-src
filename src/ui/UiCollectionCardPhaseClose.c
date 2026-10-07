// bdc 0x08982f3c UiCollectionCardPhaseClose
#include "bdc.h"

/* Last phase (slot `0x08a9e854` of table `0x08a9e7e0`) of the UiCollectionCard collection screen:
   requests the screen's close (`closeRequested`, `+0x4c` = 1). */

void UiCollectionCardPhaseClose(UiCollectionCard *self)

{
  (self->base).closeRequested = '\x01';
  return;
}

