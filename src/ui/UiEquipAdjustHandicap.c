// bdc 0x0895feb0 UiEquipAdjustHandicap
#include "bdc.h"

/* Changes player `player`'s handicap `handicap[player]` on the UiEquip Bakugan/gear loadout screen
   (task 302, `UiEquipCtor`) with the shoulder buttons (repeat bits 0x100 L: -50 down to 50, 0x200
   R: +50 up to 150); returns 1 when it changed. */

s32 UiEquipAdjustHandicap(UiEquip *self, u8 player)
{
  PadState *pad = self->base.pad;

  if (pad->repeat & 0x100) {
    if (self->handicap[player] != 50) {
      self->handicap[player] -= 50;
      return 1;
    }
  } else if (pad->repeat & 0x200) {
    if (self->handicap[player] != 150) {
      self->handicap[player] += 50;
      return 1;
    }
  }
  return 0;
}
