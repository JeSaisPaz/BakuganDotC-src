// bdc 0x08913090 UiUpgradeStartPhase
#include "bdc.h"

/* Phase 0 of the upgrade screen: immediately advances to phase 1. */

void UiUpgradeStartPhase(UiUpgrade *self)

{
  (self->base).phaseStep = 0;
  (self->base).phase = (self->base).phase + 1;
  return;
}

