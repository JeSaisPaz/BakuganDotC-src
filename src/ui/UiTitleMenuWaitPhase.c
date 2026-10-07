// bdc 0x08950614 UiTitleMenuWaitPhase
#include "bdc.h"

/* Phase 1 of `UiTitleMenu`: waits for the active fader to finish, builds the
   sprites (`UiTitleMenuBuildSprites`), then advances to the main phase. */

void UiTitleMenuWaitPhase(UiScreen *screen)

{
  if (screen->phaseStep == 0) {
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      UiTitleMenuBuildSprites(screen);
      screen->phaseStep = screen->phaseStep + 1;
    }
  }
  else {
    screen->phaseStep = 0;
    screen->phase = screen->phase + 1;
  }
  return;
}
