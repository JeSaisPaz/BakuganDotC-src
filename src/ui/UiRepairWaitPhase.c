// bdc 0x0890fa34 UiRepairWaitPhase
#include "bdc.h"

/* Phase 0 of the card repair screen: waits two frames on `phaseStep`, then advances the phase. */

void UiRepairWaitPhase(UiScreen *screen)

{
  s32 step = screen->phaseStep;

  if (step >= 0 && step < 2) {
    screen->phaseStep = step + 1;
    return;
  }
  screen->phaseStep = 0;
  screen->phase = screen->phase + 1;
}
