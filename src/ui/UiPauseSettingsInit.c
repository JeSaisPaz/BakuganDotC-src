// bdc 0x089ab7cc UiPauseSettingsInit
#include "bdc.h"

/* Class init of `UiPauseSettings` (from `UiPauseSettingsCtor`): cursor 0,
   no preview voice handle (`+0xb7c`), loads and snapshots the values
   (`UiPauseSettingsLoadValues`, `UiPauseSettingsSnapshotValues`) and sets the item count
   `+0xbb3` to 7 when `UiPauseSettingsHasExtraEntry`, else 6. */

void UiPauseSettingsInit(UiPauseSettings *self)

{
  u8 count;

  self->cursor = '\0';
  self->previewVoice = 0;
  UiPauseSettingsLoadValues(self);
  UiPauseSettingsSnapshotValues(self);
  count = 6;
  if (UiPauseSettingsHasExtraEntry(self) == 1) {
    count = 7;
  }
  self->itemCount = count;
  return;
}

