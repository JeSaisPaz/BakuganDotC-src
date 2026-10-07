// bdc 0x0894f080 UiNetMenuSetupPhase
#include "bdc.h"

/* Phase 1 of `UiNetMenu` (phase table `0x08a9d360`): step 0 builds the sprites
   (`UiNetMenuBuildSprites`); step 1 prints the help for the current choice
   (`UiNetMenuSetHelpText`) and advances to the main phase. */

void UiNetMenuSetupPhase(UiScreen *screen)

{
  if (screen->phaseStep == 0) {
    UiNetMenuBuildSprites(screen);
    screen->phaseStep = screen->phaseStep + 1;
  }
  else {
    UiNetMenuSetHelpText(screen,(u8)((UiNetMenu *)screen)->choice);
    screen->phaseStep = 0;
    screen->phase = screen->phase + 1;
  }
  return;
}
