// bdc 0x089ade88 UiPauseSettingsRefreshRowValue
#include "bdc.h"

/* Redraws the value of the row under the cursor (`+0x74`) of
   `UiPauseSettings`: rows 0..2 (BGM / voice / SE volume) redraw their slider
   sprite (`data+0x90 + row*4`) with `UiPauseSettingsSetBarWidth(screen, sprite, UiPauseSettingsGetItemValue(screen, row))`; row
   3 shows the toggle state (`UiPauseSettingsSetToggleCell` on sprite 61 with profile byte
   `+0x6ab`). */

void UiPauseSettingsRefreshRowValue(UiPauseSettings *self)
{
  GfxSprite **sprites = (GfxSprite **)(self->base).data;
  s32 cursor = self->cursor;

  if (cursor < 3) {
    GfxSprite *bar = sprites[cursor + 0x90 / 4];
    u8 value = UiPauseSettingsGetItemValue(self, cursor);
    UiPauseSettingsSetBarWidth(self, bar, value);
  } else {
    GfxSprite *cell = sprites[0xf4 / 4];
    SaveProfile *profile = SaveGetProfile();
    UiPauseSettingsSetToggleCell(self, cell, profile->data->adviceOff);
  }
}
