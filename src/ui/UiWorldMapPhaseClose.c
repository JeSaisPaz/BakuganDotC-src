// bdc 0x08998ac4 UiWorldMapPhaseClose
#include "bdc.h"

/* Phase 3 of `UiWorldMap`: requests its own close once the shared-background task
   320 (`UiSharedBgCtor`) is gone. */

void UiWorldMapPhaseClose(UiScreen *screen)
{
  if (CoreTaskExists(0x140) == 0) {
    screen->closeRequested = 1;
  }
}
