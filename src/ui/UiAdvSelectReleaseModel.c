// bdc 0x0891887c UiAdvSelectReleaseModel
#include "bdc.h"

/* Releases the Bakugan model `+0x914` of the adventure partner-select screen (`UiAdvSelectCtor`,
   task 376; cursor `+0x74`, candidates `+0x8a0` as 4-byte `{id, partner, locked, ?}` slots)
   (`CoreObjectDeferDelete`) and clears the pointer. */

void UiAdvSelectReleaseModel(UiAdvSelect *self)

{
  if (self->model != (CoreObject *)0x0) {
    CoreObjectDeferDelete(self->model,0);
    self->model = (void *)0x0;
  }
  return;
}

