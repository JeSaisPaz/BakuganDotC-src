// bdc 0x08929670 UiHologramViewExitPhase
#include "bdc.h"

/* Phase 3 of the hologram view: sets `closeRequested` (`+0x4c`). */

void UiHologramViewExitPhase(UiHologramView *self)

{
  (self->base).closeRequested = '\x01';
  return;
}

