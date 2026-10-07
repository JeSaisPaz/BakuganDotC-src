// bdc 0x0895133c UiTitleUpdateBackground
#include "bdc.h"

/* Steps the background animations of `UiTitle` while their intro runs: returns 1 once
   the intro is over (`UiScreenAnimIsDone` non-zero; then only `title_repert` keeps stepping);
   before that, Cross skips the intro by jumping `title.fab`, `title_haikei.fab` and
   `title_logo.fab` to their last frames (`GfxFabAdvanceTo`), otherwise they are stepped normally.
   Returns 0 while the intro runs or without animations. */

s32 UiTitleUpdateBackground(UiScreen *screen)

{
  UiTitle *title = (UiTitle *)screen;
  GfxFab **fabs = (GfxFab **)screen->bgData;
  s32 done = 0;

  if (fabs != (GfxFab **)0x0) {
    if (!UiScreenAnimIsDone(screen, 0xffffffff, 0)) {
      if ((screen->pad->pressed & 0x4000) != 0) {
        GfxFabAdvanceTo(fabs[0], title->skipFrameA);
        GfxFabAdvanceTo(fabs[1], title->skipFrameB);
        GfxFabAdvanceTo(fabs[3], title->skipFrameC);
      } else {
        GfxFabListUpdate(&screen->bgAnimList);
      }
    } else {
      GfxFabUpdate(fabs[2]);
      done = 1;
    }
  }
  return done;
}
