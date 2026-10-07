// bdc 0x089abfe0 UiPauseSettingsResetDefaults
#include "bdc.h"

/* Resets the options to their defaults: BGM, voice and SE volume 10 (`SaveProfileSetBgmVolume`,
   `SaveProfileSetVoiceVolume`, `SaveProfileSetSeVolume`), reloads the sliders
   (`UiPauseSettingsLoadValues`) and turns advisor comments on (profile `+0x6ab = 0`) when
   `UiPauseSettingsAdviceAvailable`. */

void UiPauseSettingsResetDefaults(UiPauseSettings *self)

{
  SaveProfileSetBgmVolume(SaveGetProfile(),10);
  SaveProfileSetVoiceVolume(SaveGetProfile(),10);
  SaveProfileSetSeVolume(SaveGetProfile(),10);
  UiPauseSettingsLoadValues(self);
  if (UiPauseSettingsAdviceAvailable(self) == 1) {
    SaveGetProfile()->data->adviceOff = 0;
  }
  return;
}

