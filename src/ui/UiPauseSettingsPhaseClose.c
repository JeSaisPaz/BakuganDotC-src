// bdc 0x089abf0c UiPauseSettingsPhaseClose
#include "bdc.h"

/* Last phase (3) of `UiPauseSettings`: requests its own close once the
   shared-background task 320 is gone. */

void UiPauseSettingsPhaseClose(UiPauseSettings *self)
{
  if (CoreTaskExists(0x140) == 0) {
    self->base.closeRequested = 1;
  }
}
