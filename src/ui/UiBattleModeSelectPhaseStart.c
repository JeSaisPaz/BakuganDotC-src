// bdc 0x089affa4 UiBattleModeSelectPhaseStart
#include "bdc.h"

/* Phase 0 of `UiBattleModeSelect`: does nothing but advance to phase 1. */

void UiBattleModeSelectPhaseStart(UiBattleModeSelect *self)

{
  if ((self->base).phaseStep == 0) {
    (self->base).phaseStep = 1;
  }
  (self->base).phaseStep = 0;
  (self->base).phase = (self->base).phase + 1;
  return;
}

