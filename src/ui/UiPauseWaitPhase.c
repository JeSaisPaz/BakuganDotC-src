// bdc 0x08910a10 UiPauseWaitPhase
#include "bdc.h"

/* Phase 0 of the pause menu: every path of the original (its `phaseStep` increments are dead
   stores) resets `phaseStep` to 0 and advances to phase 1. */

void UiPauseWaitPhase(UiPause *self)
{
    self->base.phaseStep = 0;
    self->base.phase = self->base.phase + 1;
}
