// bdc 0x08910a64 UiPauseExitPhase
#include "bdc.h"

/* Phase 3 of the pause menu: sets `closeRequested` (`+0x4c`) so `UiScreenUpdateCommon` removes
   the screen. */

void UiPauseExitPhase(UiPause *self)

{
  (self->base).closeRequested = '\x01';
  return;
}

