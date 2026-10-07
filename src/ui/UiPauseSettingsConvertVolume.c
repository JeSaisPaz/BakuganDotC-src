// bdc 0x089ab654 UiPauseSettingsConvertVolume
#include "bdc.h"

/* Converts between the profile's stored volume and the slider value: `toProfile == 0` maps a stored
   level to the slider (`v > 1 ? v - 1 : 0`), `toProfile != 0` maps a slider value back (`v != 0 ? v
   + 1 : 0`). */

u8 UiPauseSettingsConvertVolume(UiPauseSettings *self, u8 toProfile, u8 value)
{
    if (toProfile == 0) {
        return (value < 2) ? 0 : (u8)(value - 1);
    }
    return (value == 0) ? 0 : (u8)(value + 1);
}
