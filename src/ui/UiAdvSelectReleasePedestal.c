// bdc 0x089188b4 UiAdvSelectReleasePedestal
#include "bdc.h"

/* Releases the pedestal model `+0x918` of the adventure partner-select screen (`UiAdvSelectCtor`,
   task 376; cursor `+0x74`, candidates `+0x8a0` as 4-byte `{id, partner, locked, ?}` slots)
   (`CoreObjectDeferDelete`) and clears the pointer. */

void UiAdvSelectReleasePedestal(UiAdvSelect *self)

{
  if (self->pedestal != NULL) {
    CoreObjectDeferDelete(&self->pedestal->base,0);
    self->pedestal = NULL;
  }
  return;
}

