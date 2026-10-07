// bdc 0x08960674 UiEquipMoveGearCursor
#include "bdc.h"

/* On the list row, moves the equipment-panel entry `+0x5024` of the UiEquip Bakugan/gear loadout
   screen (task 302, `UiEquipCtor`) left/right (repeat bits 0x80/0x20) with wrap-around over the
   four entries; returns 1 when it moved; the active row is `+0x5025` (in network mode —
   `SaveGetProfileFlag0` set — the local player's row `+0x5026[+0x52a0]`); row 0 = equipment
   list, row 1 = OK button. */

s32 UiEquipMoveGearCursor(UiEquip *self)
{
  s8 row = self->activeRow;
  PadState *pad;

  if (SaveGetProfileFlag0()) {
    row = self->gearRowFlags[*(int *)self->localPlayer];
  }
  if (row == 0) {
    pad = self->base.pad;
    if ((s8)pad->repeat & 0x80) {
      self->panelEntry = (self->panelEntry == 0) ? 3 : (s8)(self->panelEntry - 1);
      return 1;
    }
    if (pad->repeat & 0x20) {
      self->panelEntry = (self->panelEntry == 3) ? 0 : (s8)(self->panelEntry + 1);
      return 1;
    }
  }
  return 0;
}
