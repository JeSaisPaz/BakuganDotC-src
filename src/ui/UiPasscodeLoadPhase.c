// bdc 0x0893da80 UiPasscodeLoadPhase
#include "bdc.h"

/* Phase 0 of `UiPasscode` (phase table `0x08a9ce20`): step 0 builds the sprites
   (`UiPasscodeBuildSprites`), step 1 advances to phase 1. */

void UiPasscodeLoadPhase(UiScreen *screen)

{
  if (screen->phaseStep == 0) {
    UiPasscodeBuildSprites(screen);
    screen->phaseStep = screen->phaseStep + 1;
  }
  else {
    screen->phaseStep = 0;
    screen->phase = screen->phase + 1;
  }
  return;
}

