// bdc 0x0896022c UiEquipBuildGearChoices
#include "bdc.h"

/* Fills player `player`'s equipment choices on the UiEquip Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`) for the hovered Bakugan (`UiEquipMapBakuganIndex(self, 0, gridCursor)` = id): for its
   four entries `id*4 + i - 4` whose bit is set in the profile bit field `newItems` (+0x5c8) stores the
   entry id in `panelCards[player*4 + i]` (0xff = empty); the first two such entries are preselected
   (bit 0 of `panelCardFlags[...]`, bit 1 on the first) and copied to `gearPick[player]`. */

void UiEquipBuildGearChoices(UiEquip *self, u8 player)
{
  SaveProfile *profile;
  int entry;
  int bit;
  int idx;
  u8 count;
  u8 first;
  u8 id;

  memset(&self->panelCards[player * 4], 0xff, 4);
  memset(&self->panelCardFlags[player * 4], 0, 4);
  /* 4 bytes: also clears the next player's pair, as the original does */
  memset(self->gearPick[player], 0xff, 4);
  count = 0;
  first = 0;
  entry = 0;
  do {
    profile = SaveGetProfile();
    id = UiEquipMapBakuganIndex(self, 0, (u8)self->gridCursor);
    bit = id * 4 + entry - 4;
    if ((u8)(profile->data->newItems[bit / 8] & (1 << (bit % 8))) != 0) {
      id = UiEquipMapBakuganIndex(self, 0, (u8)self->gridCursor);
      idx = player * 4 + entry;
      self->panelCards[idx] = (u8)(id * 4 + entry - 4);
      if (count < 2) {
        self->gearPick[player][count] = self->panelCards[idx];
        self->panelCardFlags[idx] |= 1;
        if (first == 0) {
          self->panelCardFlags[idx] |= 2;
          first++;
        }
      }
      count++;
    }
    entry++;
  } while (entry < 4);
}
