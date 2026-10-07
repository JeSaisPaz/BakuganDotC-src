// bdc 0x08944ffc UiStaffCreditSetupPhase
#include "bdc.h"

/* Phase 1 of `UiStaffCredit` (phase table `0x08a9d078`): step 0 starts the
   `"main_bg.fab"` background (`UiSharedAnimStart`), allocates the 0x7c-byte sprite table (`data`),
   creates the 0x31 layout sprites (`UiLayoutCreateSprites`), initialises them
   (`UiStaffCreditInitSprites`) and the line printers (`UiStaffCreditCreateLinePrinters`); step
   1 waits for the fade (`UiStaffCreditFadeIsFinished`) and enters phase 2. */

void UiStaffCreditSetupPhase(UiScreen *screen)
{
  bool prevFromLow;
  int step;

  step = screen->phaseStep;
  if (step < 1) {
    if (step >= 0) {
      UiSharedAnimStart(1.0f, 0.0f, 0.0f, screen, "main_bg.fab", 0, 0);
      MemLock();
      prevFromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      screen->data = MemAlloc(0x7c, NULL, 0);
      MemSetAllocFromLow(prevFromLow);
      MemUnlock();
      UiLayoutCreateSprites(screen->spriteLayer, (GfxSprite **)screen->data, 0x31);
      UiStaffCreditInitSprites(screen);
      UiStaffCreditCreateLinePrinters(screen);
      screen->phaseStep = 1;
    }
  } else if (step < 2 && UiStaffCreditFadeIsFinished()) {
    screen->phase = 2;
    screen->phaseStep = 0;
  }
}
