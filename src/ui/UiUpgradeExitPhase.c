// bdc 0x089130a4 UiUpgradeExitPhase
#include "bdc.h"

/* Phase 3 of the upgrade screen: sets `closeRequested` (`+0x4c`). */

void UiUpgradeExitPhase(UiUpgrade *self)

{
  (self->base).closeRequested = '\x01';
  return;
}

