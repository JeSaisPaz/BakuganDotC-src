// bdc 0x0896061c UiEquipNetMoveGearRow
#include "bdc.h"

/* Network version of `UiEquipMoveGearRow` on the UiEquip Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`): switches the local player's row `+0x5026[+0x52a0]` (down 0x40 → OK button, up
   0x10 → list); returns 1 when it changed. */

s32 UiEquipNetMoveGearRow(UiEquip *self)
{
  s32 player = *(s32 *)self->localPlayer;
  PadState *pad = self->base.pad;

  if (self->gearRowFlags[player] == 0) {
    if ((pad->repeat & 0x40) != 0) {
      self->gearRowFlags[player] = 1;
      return 1;
    }
  } else if ((pad->repeat & 0x10) != 0) {
    self->gearRowFlags[player] = 0;
    return 1;
  }
  return 0;
}
