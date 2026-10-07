// bdc 0x08969a5c UiCardEquipPhaseStart
#include "bdc.h"

/* Phase 0 of `UiCardEquip`: waits one frame and advances to phase 1. */

void UiCardEquipPhaseStart(UiCardEquip *self)

{
  if ((self->base).phaseStep == 0) {
    (self->base).phaseStep = 1;
    return;
  }
  (self->base).phaseStep = 0;
  (self->base).phase = (self->base).phase + 1;
  return;
}

