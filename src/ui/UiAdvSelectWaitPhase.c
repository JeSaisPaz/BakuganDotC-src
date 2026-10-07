// bdc 0x08918404 UiAdvSelectWaitPhase
#include "bdc.h"

/* Phase 1 of the adventure select screen: waits one frame, then advances. */

void UiAdvSelectWaitPhase(UiAdvSelect *self)

{
  if ((self->base).phaseStep == 0) {
    (self->base).phaseStep = 1;
    return;
  }
  (self->base).phaseStep = 0;
  (self->base).phase = (self->base).phase + 1;
  return;
}

