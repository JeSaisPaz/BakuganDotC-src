// bdc 0x0895043c UiTitleMenuLoadPhase
#include "bdc.h"

/* Phase 0 of `UiTitleMenu` (phase table `0x08a9d3b8`): step 0 creates the package
   `package` (+0x6c, `IoLzsPackageCtor`) if missing and starts loading `"data/sysmenu.lzs"`
   (`IoLzsPackageStartLoad`), advancing once the load is started; step 1 polls it
   (`IoLzsPackagePoll`) and when loaded starts the looping `"title.fab"` background in slot 0
   (depth 10, `UiScreenAnimCreate`, `GfxFabListUpdate`) and advances; any other step (also a
   negative one) resets the step and advances the phase. */

void UiTitleMenuLoadPhase(UiScreen *screen)
{
  UiTitleMenu *menu = (UiTitleMenu *)screen;
  int step = screen->phaseStep;

  if (step > 0) {
    if (step < 2) {
      if (IoLzsPackagePoll(menu->package, 1) == 0) {
        return;
      }
      UiScreenAnimCreate(screen, "title.fab", 0);
      ((GfxFab **)screen->bgData)[0]->loop = 1;
      ((GfxFab **)screen->bgData)[0]->depth = 10.0f;
      GfxFabListUpdate(&screen->bgAnimList);
      screen->phaseStep = screen->phaseStep + 1;
      return;
    }
  }
  else if (step >= 0) {
    if (menu->package == NULL) {
      IoLzsPackage *package = NULL;
      bool fromLow;
      IoLzsPackage *self;

      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      self = MemAlloc(sizeof(IoLzsPackage), NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      if (self != NULL) {
        IoLzsPackageCtor(self);
        package = self;
      }
      menu->package = package;
    }
    if (IoLzsPackageStartLoad(menu->package, "data/sysmenu.lzs", 10, 1, 0) == 0) {
      return;
    }
    screen->phaseStep = screen->phaseStep + 1;
    return;
  }
  step = screen->phase;
  screen->phaseStep = 0;
  screen->phase = step + 1;
}
