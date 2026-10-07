// bdc 0x089a8534 UiMainMenuPhaseClose
#include "bdc.h"

/* Last phase (6) of `UiMainMenu`: waits until the shared-background task 320
   (`UiSharedBgCtor`) is gone, then requests the screen's own close (`closeRequested`, +0x4c). */

void UiMainMenuPhaseClose(UiMainMenu *self)

{
  s32 exists;
  
  exists = CoreTaskExists(0x140);
  if (exists == 0) {
    (self->base).closeRequested = '\x01';
  }
  return;
}

