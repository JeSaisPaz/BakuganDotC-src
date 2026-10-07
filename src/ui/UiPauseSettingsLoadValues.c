// bdc 0x089ab698 UiPauseSettingsLoadValues
#include "bdc.h"

/* Loads the three slider values from the profile options: BGM → `sliders[0]`, voice →
   `sliders[1]`, SE → `sliders[2]`, through `UiPauseSettingsConvertVolume`. */

void UiPauseSettingsLoadValues(UiPauseSettings *self)
{
  self->sliders[0] = UiPauseSettingsConvertVolume(self, 0, SaveGetProfile()->data->bgmVolume);
  self->sliders[1] = UiPauseSettingsConvertVolume(self, 0, SaveGetProfile()->data->voiceVolume);
  self->sliders[2] = UiPauseSettingsConvertVolume(self, 0, SaveGetProfile()->data->seVolume);
}
