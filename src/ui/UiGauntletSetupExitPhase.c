// bdc 0x08932f78 UiGauntletSetupExitPhase
#include "bdc.h"

/* Phase 3 of the gauntlet setup screen: sets `closeRequested` (`+0x4c`). */

void UiGauntletSetupExitPhase(UiGauntletSetup *self)

{
  (self->base).closeRequested = '\x01';
  return;
}

