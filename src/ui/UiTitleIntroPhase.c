// bdc 0x08951130 UiTitleIntroPhase
#include "bdc.h"

/* Phase 1 of `UiTitle`: builds the sprites once (`UiTitleBuildSprites`); when the
   intro animation has finished (`UiLoadingIsOpen` = 0) plays the title BGM 0x1a on player 0 (if it
   exists) and enters the press-start phase (`UiTitlePressStartPhase`). */

void UiTitleIntroPhase(UiScreen *screen)
{
  if (screen->phaseStep == 0) {
    UiTitleBuildSprites(screen);
    screen->phaseStep = screen->phaseStep + 1;
  }
  if (!UiLoadingIsOpen()) {
    if (SndBgmPlayerExists(0)) {
      SndBgmPlayerPlayTrack(SndBgmPlayerGet(0), 0x1a, 0, 0);
    }
    screen->phaseStep = 0;
    screen->phase = screen->phase + 1;
  }
}
