// bdc 0x088d2804 UiFieldHudStopPhase
#include "bdc.h"

/* Phase 4 of the field HUD: sets the phase to 5 (past the table), so no phase handler runs any
   more. */

void UiFieldHudStopPhase(UiFieldHud *self)

{
  (self->base).phase = 5;
  return;
}

