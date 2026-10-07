// bdc 0x089b16f8 UiBattleModeSelectStepFlash
#include "bdc.h"

/* Waits for the confirm flash of `UiBattleModeSelect`: steps it
   (`UiFlashStep`) and returns true once it has finished. `screen` is unused. */

bool UiBattleModeSelectStepFlash(UiBattleModeSelect *self)

{
  
  if (UiFlashStep(0)) {
    return true;
  }
  return false;
}

