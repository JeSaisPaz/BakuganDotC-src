// bdc 0x08961fc4 UiEquipCheckHelpButton
#include "bdc.h"

/* Checks the help button (pressed bit 0x8000, Square) in the equipment panel of the UiEquip
   Bakugan/gear loadout screen (task 302, `UiEquipCtor`): returns 0 when not pressed, 1 when the
   list row is active and the entry under the cursor is not empty (`panelCards[player*4 + panelEntry]`
   != 0xff), else 2; the active row is `activeRow` (in network mode — `SaveGetProfileFlag0` set —
   the local player's row `gearRowFlags[localPlayer]`); row 0 = equipment list, row 1 = OK button. */

s32 UiEquipCheckHelpButton(UiEquip *self)

{
  s8 row;

  if ((self->base.pad->pressed & 0x8000) == 0) {
    return 0;
  }
  row = self->activeRow;
  if (SaveGetProfileFlag0() != 0) {
    row = self->gearRowFlags[*(s32 *)self->localPlayer];
  }
  if (row == 0 && self->panelCards[self->panelEntry + self->editPlayer * 4] != 0xff) {
    return 1;
  }
  return 2;
}
