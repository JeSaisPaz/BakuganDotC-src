// bdc 0x0892962c UiHologramViewWaitPhase
#include "bdc.h"

/* Phase 1 of the hologram view: waits two frames, then advances. */

void UiHologramViewWaitPhase(UiHologramView *self)

{
  int step;
  
  step = (self->base).phaseStep;
  if (step < 1) {
    if (-1 < step) {
      (self->base).phaseStep = step + 1;
      return;
    }
  }
  else if (step < 2) {
    (self->base).phaseStep = step + 1;
    return;
  }
  (self->base).phaseStep = 0;
  (self->base).phase = (self->base).phase + 1;
  return;
}

