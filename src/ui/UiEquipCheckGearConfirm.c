// bdc 0x08960a10 UiEquipCheckGearConfirm
#include "bdc.h"

/* Handles the confirm button (pressed bit 0x4000, Cross) in the equipment panel of the UiEquip
   Bakugan/gear loadout screen (task 302, `UiEquipCtor`) (offline): returns 0 when not pressed, 1
   on the OK row, 2 when the entry under the cursor is empty (`+0x4fb8[player*4 + +0x5024]` ==
   0xff), otherwise toggles that entry (`UiEquipToggleGearChoice`, add if not in use) and returns
   1. */

s32 UiEquipCheckGearConfirm(UiEquip *self)

{
  u8 player;
  u8 entry;
  int idx;

  if ((self->base.pad->pressed & 0x4000) == 0) {
    return 0;
  }
  if (self->activeRow != 0) {
    return 1;
  }
  player = self->editPlayer;
  entry = self->panelEntry;
  idx = (s8)entry + (s8)player * 4;
  if (self->panelCards[idx] == 0xff) {
    return 2;
  }
  if ((self->panelCards[idx + 0x10] & 1) == 0) {
    UiEquipToggleGearChoice(self, true, player, entry);
    return 1;
  }
  UiEquipToggleGearChoice(self, false, player, entry);
  return 1;
}
