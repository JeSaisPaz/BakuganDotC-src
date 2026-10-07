// bdc 0x089356fc UiGauntletSetupConfirmFlashDone
#include "bdc.h"

/* Advances the confirm flash channels of `UiGauntletSetup` (`UiFlashStep`:
   channels 0 and 1 on the slots, channel 0 on the OK button) and returns 1 once the flash has
   finished, else 0. */

s32 UiGauntletSetupConfirmFlashDone(UiGauntletSetup *self)
{
  if (self->focusArea == 0) {
    UiFlashStep(0);
    if (UiFlashStep(1) != 0) {
      return 1;
    }
  }
  else if (UiFlashStep(0) != 0) {
    return 1;
  }
  return 0;
}
