// bdc 0x089512a0 UiTitleClosePhase
#include "bdc.h"

/* Phase 6 of `UiTitle`: when the menu result is 0 or 1 waits one extra step, then
   sets fader `+0x10` to 20000 and requests the close (`+0x4c`). */

void UiTitleClosePhase(UiScreen *screen)

{
  s32 result;
  GfxFader *fader;
  int step;
  
  step = screen->phaseStep;
  if (step == 100) {
    fader = GfxGetActiveFader();
    fader->sortKey = 20000.0f;
    screen->closeRequested = '\x01';
  }
  else if (step == 10) {
    screen->phaseStep = 100;
  }
  else if (step == 0) {
    result = UiGetMenuResult(screen);
    if ((result < 0) || (1 < result)) {
      screen->phaseStep = 100;
    }
    else {
      screen->phaseStep = 10;
    }
  }
  return;
}

