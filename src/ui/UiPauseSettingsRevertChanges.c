// bdc 0x089ae0f4 UiPauseSettingsRevertChanges
#include "bdc.h"

/* Restores the settings of `UiPauseSettings` from the snapshot taken when it
   opened: BGM, voice and SE volumes from `+0xbb8/+0xbb9/+0xbba` (converted with
   `UiPauseSettingsConvertVolume(screen, 1, v)`, `SaveProfileSetBgmVolume`, `SaveProfileSetVoiceVolume`,
   `SaveProfileSetSeVolume`) and the toggle byte `*profile + 0x6ab` from `+0xbbb`. */

void UiPauseSettingsRevertChanges(UiPauseSettings *self)
{
  SaveProfile *profile;

  profile = SaveGetProfile();
  SaveProfileSetBgmVolume(profile, UiPauseSettingsConvertVolume(self, 1, self->savedValues[0]));
  profile = SaveGetProfile();
  SaveProfileSetVoiceVolume(profile, UiPauseSettingsConvertVolume(self, 1, self->savedValues[1]));
  profile = SaveGetProfile();
  SaveProfileSetSeVolume(profile, UiPauseSettingsConvertVolume(self, 1, self->savedValues[2]));
  SaveGetProfile()->data->adviceOff = self->savedValues[3];
}
