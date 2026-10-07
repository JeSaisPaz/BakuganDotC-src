// bdc 0x0890ea30 UiConfirmDialogExitPhase
#include "bdc.h"

/* Phase 3 of the yes/no confirm dialog: sets the UiScreen close flag `+0x4c`, so the next update
   removes the task. */

void UiConfirmDialogExitPhase(UiScreen *screen)

{
  screen->closeRequested = '\x01';
  return;
}

