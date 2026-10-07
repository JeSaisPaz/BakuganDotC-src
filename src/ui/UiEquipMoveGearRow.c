// bdc 0x089605d0 UiEquipMoveGearRow
#include "bdc.h"

/* Switches the equipment-panel row `+0x5025` of the UiEquip Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`) with the pad: down (repeat bit 0x40) from the list to the OK button, up (0x10)
   back; returns 1 when it changed. */

s32 UiEquipMoveGearRow(UiEquip *self)
{
  PadState *pad = self->base.pad;

  if (self->activeRow == 0) {
    if ((pad->repeat & 0x40) != 0) {
      self->activeRow = 1;
      return 1;
    }
  } else if ((pad->repeat & 0x10) != 0) {
    self->activeRow = 0;
    return 1;
  }
  return 0;
}
