// bdc 0x08974890 UiCollectionMenuPhaseClose
#include "bdc.h"

/* Phase 8 of `UiCollectionMenu`: once the shared-background keeper (task
   320) is gone, requests the screen's removal (`+0x4c` = 1). */

void UiCollectionMenuPhaseClose(UiCollectionMenu *self)

{
  if (CoreTaskExists(0x140) == 0) {
    (self->base).closeRequested = '\x01';
  }
  return;
}

