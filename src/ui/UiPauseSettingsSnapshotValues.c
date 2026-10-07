// bdc 0x089ab70c UiPauseSettingsSnapshotValues
#include "bdc.h"

/* Copies the current slider values to `savedValues[0..2]` and the advisor toggle (profile `adviceOff`) to
   `savedValues[3]`, the state that a cancel restores. */

void UiPauseSettingsSnapshotValues(UiPauseSettings *self)

{
  int i;

  for (i = 0; i < 3; i++) {
    self->savedValues[i] = self->sliders[i];
  }
  self->savedValues[3] = SaveGetProfile()->data->adviceOff;
}
