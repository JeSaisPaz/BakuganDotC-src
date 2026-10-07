// bdc 0x0895e1f8 UiEquipStartAllPlayerStamps
#include "bdc.h"

/* Starts the stamps (`UiEquipStartPlayerStamp`) of every player up to the current one (`+0x4cdb`)
   on the UiEquip Bakugan/gear loadout screen (task 302, `UiEquipCtor`) with the player's pick
   `+0x4cdd[p]` (the hovered grid Bakugan when none is stored); with `clear` restarts them with id
   0. */

void UiEquipStartAllPlayerStamps(UiEquip *self, bool clear)
{
  int p;

  if (clear) {
    for (p = 0; p <= self->editPlayer; p++) {
      UiEquipStartPlayerStamp(self, (u8)p, 0);
    }
  } else {
    for (p = 0; p <= self->editPlayer; p++) {
      if ((s8)self->bakuganPick[p] > 0) {
        UiEquipStartPlayerStamp(self, (u8)p, self->bakuganPick[p]);
      } else {
        UiEquipStartPlayerStamp(self, (u8)p, UiEquipMapBakuganIndex(self, 0, self->gridCursor));
      }
    }
  }
}
