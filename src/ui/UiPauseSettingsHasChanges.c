// bdc 0x089adf0c UiPauseSettingsHasChanges
#include "bdc.h"

/* Returns true when a setting of `UiPauseSettings` differs from the snapshot
   taken when the screen opened: the three slider values `+0xbb0..0xbb2` against `+0xbb8..0xbba`,
   and the profile toggle byte `*profile + 0x6ab` against `+0xbbb`. */

bool UiPauseSettingsHasChanges(UiPauseSettings *self)
{
  s32 i = 0;
  bool slider = true;

  do {
    if (slider) {
      if (self->sliders[i] != self->savedValues[i]) {
        return true;
      }
    } else {
      SaveProfile *profile = SaveGetProfile();
      if (profile->data->adviceOff != self->savedValues[i]) {
        return true;
      }
    }
    i = i + 1;
    slider = i < 3;
  } while (i < 4);
  return false;
}
