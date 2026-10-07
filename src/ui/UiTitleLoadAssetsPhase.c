// bdc 0x08950d90 UiTitleLoadAssetsPhase
#include "bdc.h"

/* Phase 0 of `UiTitle` (phase table `0x08a9d3f8`), stepped by `phaseStep`:
   step 0 cancels and stops BGM channel 0; step 1 waits until task 0x2756 is gone; step 2
   preloads stream 0x1a (title music, `SndStreamFilePreload`); step 3 waits until it is loaded,
   then creates the background animations `"title.fab"`, `"title_haikei.fab"`,
   `"title_repert.fab"` (looping) and `"title_logo.fab"` in slots 0..3, sets their depths and keeps
   their frame counts (`GfxFabGetFrameCount`) in `skipFrameA`/`skipFrameB`/`repertFrame`/
   `skipFrameC`. Step 4 (or a negative step) resets `phaseStep` and advances `phase`. */

void UiTitleLoadAssetsPhase(UiScreen *screen)
{
  UiTitle *title = (UiTitle *)screen;
  int step;

  step = screen->phaseStep;
  if (step < 0 || step > 3) {
    screen->phaseStep = 0;
    screen->phase = screen->phase + 1;
    return;
  }
  if (step < 2) {
    if (step < 1) {
      SndBgmCancelChannel(0);
      SndBgmQueueStop(0.0f, 0);
      screen->phaseStep = screen->phaseStep + 1;
    }
    if (CoreTaskExists(0x2756) != 0) {
      return;
    }
    screen->phaseStep = screen->phaseStep + 1;
  }
  if (step < 3) {
    SndStreamFilePreload(0x1a);
    screen->phaseStep = screen->phaseStep + 1;
  }
  if (!SndStreamFileIsLoaded(0x1a)) {
    return;
  }

  UiScreenAnimCreate(screen, "title.fab", 0);

  ((GfxFab **)screen->bgData)[0]->loop = 0;
  ((GfxFab **)screen->bgData)[0]->depth = 10.0f;
  title->skipFrameA = GfxFabGetFrameCount(((GfxFab **)screen->bgData)[0]);

  UiScreenAnimCreate(screen, "title_haikei.fab", 1);
  ((GfxFab **)screen->bgData)[1]->loop = 0;
  ((GfxFab **)screen->bgData)[1]->depth = 8.0f;
  title->skipFrameB = GfxFabGetFrameCount(((GfxFab **)screen->bgData)[1]);

  UiScreenAnimCreate(screen, "title_repert.fab", 2);
  ((GfxFab **)screen->bgData)[2]->loop = 1;
  ((GfxFab **)screen->bgData)[2]->depth = 9.0f;
  title->repertFrame = GfxFabGetFrameCount(((GfxFab **)screen->bgData)[2]);

  UiScreenAnimCreate(screen, "title_logo.fab", 3);
  ((GfxFab **)screen->bgData)[3]->loop = 0;
  ((GfxFab **)screen->bgData)[3]->depth = 11.0f;
  title->skipFrameC = GfxFabGetFrameCount(((GfxFab **)screen->bgData)[3]);

  GfxFabListUpdate(&screen->bgAnimList);
  screen->phaseStep = screen->phaseStep + 1;
}
