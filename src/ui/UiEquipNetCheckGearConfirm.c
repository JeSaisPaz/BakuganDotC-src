// bdc 0x08960ad8 UiEquipNetCheckGearConfirm
#include "bdc.h"

/* Network version of `UiEquipCheckGearConfirm` on the UiEquip Bakugan/gear loadout screen (task
   302, `UiEquipCtor`): same result codes, but the OK-row test uses the local player's row
   `+0x5026[+0x52a0]`. */

s32 UiEquipNetCheckGearConfirm(UiEquip *self)

{
  u8 player;
  u8 entry;
  int idx;

  if ((self->base.pad->pressed & 0x4000) == 0) {
    return 0;
  }
  if (self->gearRowFlags[*(s32 *)self->localPlayer] != 0) {
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
