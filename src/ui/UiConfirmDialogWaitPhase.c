// bdc 0x0890e9ec UiConfirmDialogWaitPhase
#include "bdc.h"

/* Phase 0 of the yes/no confirm dialog: waits two frames (sub-state `+0x2c` 0, 1) and then advances
   the phase `+0x28`. */

void UiConfirmDialogWaitPhase(UiScreen *screen)

{
  int step = screen->phaseStep;

  if (step < 1) {
    if (-1 < step) {
      screen->phaseStep = step + 1;
      return;
    }
  } else if (step < 2) {
    screen->phaseStep = step + 1;
    return;
  }
  screen->phaseStep = 0;
  screen->phase = screen->phase + 1;
}
