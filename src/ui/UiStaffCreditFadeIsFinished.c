// bdc 0x089445ac UiStaffCreditFadeIsFinished
#include "bdc.h"

/* Returns whether the active fader has finished (`GfxFaderIsFinished` on `GfxGetActiveFader`).
    */

bool UiStaffCreditFadeIsFinished(void)

{
  return GfxFaderIsFinished(GfxGetActiveFader());
}
