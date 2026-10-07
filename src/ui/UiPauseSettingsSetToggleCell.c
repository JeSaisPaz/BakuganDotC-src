// bdc 0x089adb58 UiPauseSettingsSetToggleCell
#include "bdc.h"

/* Shows the on/off state of the toggle row of `UiPauseSettings`: sets cell
   `(0, value)` of `sprite` (`GfxSpriteSetCell`); `value` is the profile toggle byte `*profile +
   0x6ab`. `self` is unused. */

void UiPauseSettingsSetToggleCell(UiPauseSettings *self, GfxSprite *sprite, u8 value)

{
  GfxSpriteSetCell(sprite,0.0f,(float)value);
  return;
}

