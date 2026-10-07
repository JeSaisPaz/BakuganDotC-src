// bdc 0x08886794 BtlCombatResetArtSlots
#include "bdc.h"

/* Resets the special-art slots of a `BtlCombatState` after a loadout change: clears the third
   art id (`artIds[2] = -1`), zeroes the charge of every equipped slot (`artIds[i] != -1`), adds
   the number of equipped slots to `artSlotCount`, and selects slot 0's art
   (`selectedArt = artIds[0]`). */

void BtlCombatResetArtSlots(BtlCombatState *combat)
{
  int count;
  int firstArt;
  int i;

  combat->artIds[2] = -1;
  count = combat->artSlotCount;
  firstArt = combat->artIds[0];
  for (i = 0; i < 3; i++) {
    if (combat->artIds[i] != -1) {
      combat->artCharge[i] = 0.0f;
      count++;
    }
  }
  combat->artSlotCount = count;
  combat->selectedArt = firstArt;
}
