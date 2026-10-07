// bdc 0x08970190 UiOptionPhaseStart
#include "bdc.h"

/* Phase 0 of `UiOption`: waits one frame, then advances to phase 1. */

void UiOptionPhaseStart(UiOption *self)

{
  if ((self->base).phaseStep == 0) {
    (self->base).phaseStep = 1;
    return;
  }
  (self->base).phaseStep = 0;
  (self->base).phase = (self->base).phase + 1;
  return;
}

