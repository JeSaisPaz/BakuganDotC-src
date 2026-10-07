// bdc 0x089603e4 UiEquipResetGearPanelCursor
#include "bdc.h"

/* Resets the equipment-panel cursor of the UiEquip Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`): entry `+0x5024` = 0 and row `+0x5025` = 1 (the OK button), plus the two network
   rows `+0x5026/+0x5027` = 1. */

void UiEquipResetGearPanelCursor(UiEquip *self)

{
  self->panelEntry = '\0';
  self->activeRow = '\x01';
  self->gearRowFlags[0] = '\x01';
  self->gearRowFlags[1] = '\x01';
  return;
}

