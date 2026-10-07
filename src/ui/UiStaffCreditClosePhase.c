// bdc 0x0894620c UiStaffCreditClosePhase
#include "bdc.h"

/* Phase 3 of `UiStaffCredit`: waits until the shared-background task 320 is
   gone (`CoreTaskExists`), then stops the BGM (cancel channel 0, immediate stop), requests the
   close (`+0x4c`) and sets phase 4. */

void UiStaffCreditClosePhase(UiScreen *screen)

{
  if (CoreTaskExists(0x140) == 0) {
    SndBgmCancelChannel(0);
    SndBgmQueueStop(0.0f, 0);
    screen->closeRequested = 1;
    screen->phase = 4;
  }
}
