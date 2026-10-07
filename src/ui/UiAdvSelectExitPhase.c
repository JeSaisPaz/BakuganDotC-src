// bdc 0x08918430 UiAdvSelectExitPhase
#include "bdc.h"

/* Phase 3 of the adventure select screen: once the shared background screen (task 320,
   `UiSharedBgCtor`) no longer exists, sets `closeRequested` (`+0x4c`). */

void UiAdvSelectExitPhase(UiAdvSelect *self)

{
  s32 exists;
  
  exists = CoreTaskExists(0x140);
  if (exists == 0) {
    (self->base).closeRequested = '\x01';
  }
  return;
}

