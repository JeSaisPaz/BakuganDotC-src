// bdc 0x08932f4c UiGauntletSetupWaitPhase
#include "bdc.h"

/* Phase 1 of the gauntlet setup screen: waits one frame, then advances. */

void UiGauntletSetupWaitPhase(UiGauntletSetup *self)

{
  if ((self->base).phaseStep == 0) {
    (self->base).phaseStep = 1;
    return;
  }
  (self->base).phaseStep = 0;
  (self->base).phase = (self->base).phase + 1;
  return;
}

