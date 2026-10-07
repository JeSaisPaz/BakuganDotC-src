// bdc 0x08944518 UiStaffCreditPrimeFader
#include "bdc.h"

/* Initialises the fader slots if they are not ready (`GfxFaderIsReady`, `GfxFaderSlotsInit`)
   and stores `sortKey` at `+0x10` of the active fader. */

void UiStaffCreditPrimeFader(float sortKey)

{
  GfxFader *fader;

  if (!GfxFaderIsReady()) {
    GfxFaderSlotsInit((GfxFader *)0x0);
    fader = GfxGetActiveFader();
    fader->sortKey = sortKey;
  }
}
