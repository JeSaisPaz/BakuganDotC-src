// bdc 0x0892cdb4 UiBakuganSelectPhaseWait
#include "bdc.h"

/* Phase 1 of `UiBakuganSelect`: waits one frame and advances to phase 2
   (main). */

void UiBakuganSelectPhaseWait(UiBakuganSelect *self)

{
  if ((self->base).phaseStep == 0) {
    (self->base).phaseStep = 1;
    return;
  }
  (self->base).phaseStep = 0;
  (self->base).phase = (self->base).phase + 1;
  return;
}

