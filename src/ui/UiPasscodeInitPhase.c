// bdc 0x0893db60 UiPasscodeInitPhase
#include "bdc.h"

/* Phase 1 of `UiPasscode`: step 0 sets up the answer (`UiPasscodeInitAnswer`),
   step 1 advances to the main phase. */

void UiPasscodeInitPhase(UiScreen *screen)

{
  if (screen->phaseStep == 0) {
    UiPasscodeInitAnswer(screen);
    screen->phaseStep = screen->phaseStep + 1;
  }
  else {
    screen->phaseStep = 0;
    screen->phase = screen->phase + 1;
  }
  return;
}

