// bdc 0x0895ad40 UiEquipStoreSelectionWords
#include "bdc.h"

/* Stores each player's selection of `UiEquip` (`bakuganPick[i]`, signed byte) in profile
   words 3 + i (`SaveProfileSetWord`) for the `playerCount` players. */

void UiEquipStoreSelectionWords(UiEquip *self)
{
  s32 i;

  for (i = 0; i < self->playerCount; i++) {
    SaveProfileSetWord(SaveGetProfile(), i + 3, (s8)self->bakuganPick[i]);
  }
}
