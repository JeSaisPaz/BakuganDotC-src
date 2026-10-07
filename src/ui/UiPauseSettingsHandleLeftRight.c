// bdc 0x089aeb1c UiPauseSettingsHandleLeftRight
#include "bdc.h"

/* Left/Right input of `UiPauseSettings` (held D-pad bits 0x80 left / 0x20 right
   of `PadState` `buttons`; left is tested first): on rows 0..2 decrements (while > 0) or
   increments (while < 9) the slider `sliders[row]` and applies it at once
   (`SaveProfileSetBgmVolume` row 0, `SaveProfileSetVoiceVolume` row 1, `SaveProfileSetSeVolume`
   row 2, value from `UiPauseSettingsConvertVolume(self, 1, v)`); on row 3, only when
   `UiPauseSettingsAdviceAvailable` returns 1, switches the profile's `adviceOff` byte between 0
   and 1. Stores the direction in `dir` (0 left, 1 right) and returns true when a value changed,
   false otherwise (also for rows < 0 or > 3). */

bool UiPauseSettingsHandleLeftRight(UiPauseSettings *self)
{
  s8 row;
  u8 v;
  PadState *pad;
  SaveProfile *profile;

  row = self->cursor;
  if (row < 2) {
    if (row < 0) {
      return false;
    }
    pad = self->base.pad;
    if (row <= 0) {
      v = self->sliders[0];
      if (pad->buttons & 0x80) {
        if (v == 0) {
          return false;
        }
        self->sliders[0] = v - 1;
        profile = SaveGetProfile();
        SaveProfileSetBgmVolume(profile, UiPauseSettingsConvertVolume(self, 1, self->sliders[0]));
        self->dir = 0;
        return true;
      }
      if ((pad->buttons & 0x20) == 0 || v >= 9) {
        return false;
      }
      self->sliders[0] = v + 1;
      profile = SaveGetProfile();
      SaveProfileSetBgmVolume(profile, UiPauseSettingsConvertVolume(self, 1, self->sliders[0]));
      self->dir = 1;
      return true;
    }
    v = self->sliders[1];
    if (pad->buttons & 0x80) {
      if (v == 0) {
        return false;
      }
      self->sliders[1] = v - 1;
      profile = SaveGetProfile();
      SaveProfileSetVoiceVolume(profile, UiPauseSettingsConvertVolume(self, 1, self->sliders[1]));
      self->dir = 0;
      return true;
    }
    if ((pad->buttons & 0x20) == 0 || v >= 9) {
      return false;
    }
    self->sliders[1] = v + 1;
    profile = SaveGetProfile();
    SaveProfileSetVoiceVolume(profile, UiPauseSettingsConvertVolume(self, 1, self->sliders[1]));
    self->dir = 1;
    return true;
  }
  if (row < 3) {
    pad = self->base.pad;
    v = self->sliders[2];
    if (pad->buttons & 0x80) {
      if (v == 0) {
        return false;
      }
      self->sliders[2] = v - 1;
      profile = SaveGetProfile();
      SaveProfileSetSeVolume(profile, UiPauseSettingsConvertVolume(self, 1, self->sliders[2]));
      self->dir = 0;
      return true;
    }
    if ((pad->buttons & 0x20) == 0 || v >= 9) {
      return false;
    }
    self->sliders[2] = v + 1;
    profile = SaveGetProfile();
    SaveProfileSetSeVolume(profile, UiPauseSettingsConvertVolume(self, 1, self->sliders[2]));
    self->dir = 1;
    return true;
  }
  if (row >= 4 || UiPauseSettingsAdviceAvailable(self) != 1) {
    return false;
  }
  profile = SaveGetProfile();
  pad = self->base.pad;
  v = profile->data->adviceOff;
  if (pad->buttons & 0x80) {
    if (v == 0) {
      return false;
    }
    SaveGetProfile()->data->adviceOff = v - 1;
    self->dir = 0;
    return true;
  }
  if ((pad->buttons & 0x20) == 0 || v >= 1) {
    return false;
  }
  SaveGetProfile()->data->adviceOff = v + 1;
  self->dir = 1;
  return true;
}
