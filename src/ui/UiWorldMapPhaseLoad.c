// bdc 0x0899793c UiWorldMapPhaseLoad
#include "bdc.h"

/* Phase 0 of `UiWorldMap`: step 1 starts `main_bg.fab` (`UiSharedAnimStart`); step 2
   starts a 16-frame fade-in, queues BGM track 0x17 when `UiWorldMapIsRankMode` returns 1 and advances to
   phase 1. */

void UiWorldMapPhaseLoad(UiScreen *screen)
{
  GfxFader *fader;
  int step = screen->phaseStep;

  if (step > 0) {
    if (step < 2) {
      UiSharedAnimStart(10.0f, 0.0f, 0.0f, screen, "main_bg.fab", 0, 0);
      screen->phaseStep = screen->phaseStep + 1;
      return;
    }
  } else if (step >= 0) {
    screen->phaseStep = step + 1;
    return;
  }
  fader = GfxGetActiveFader();
  fader->start[0] = 0.0f;
  fader->start[1] = 0.0f;
  fader->start[2] = 0.0f;
  fader->start[3] = 1.0f;
  fader = GfxGetActiveFader();
  fader->end[0] = 0.0f;
  fader->end[1] = 0.0f;
  fader->end[2] = 0.0f;
  fader->end[3] = 0.0f;
  GfxFaderStart(GfxGetActiveFader(), 0x10);
  if (UiWorldMapIsRankMode(screen) == 1) {
    SndBgmQueuePlay(0, 0x17, 1, 0);
  }
  screen->phaseStep = 0;
  screen->phase = screen->phase + 1;
}
