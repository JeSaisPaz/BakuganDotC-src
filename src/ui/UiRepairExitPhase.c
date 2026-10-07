// bdc 0x0890ff68 UiRepairExitPhase
#include "bdc.h"

/* Phase 3 of the card repair screen: sets `closeRequested` (`+0x4c`). */

void UiRepairExitPhase(UiScreen *screen)

{
  screen->closeRequested = '\x01';
  return;
}

