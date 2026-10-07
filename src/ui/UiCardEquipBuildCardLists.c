// bdc 0x08968d98 UiCardEquipBuildCardLists
#include "bdc.h"

/* Builds the per-Bakugan card lists of the UiCardEquip ability-card loadout sub-screen (task 303,
   `UiCardEquipCtor`): for each slot p (`bakuganCount`) the four cards `bakuganIds[p]*4 + k - 4`
   that are set in the profile's `ownedItems` bit field go to `cardIds[p*4 + k]` (0xff = empty),
   with in-use flags in `cardIds[0x10 + p*4 + k]` (bit 0 in use, bit 1 first in-use card). When
   `g_lastScreenTaskId` is 0x136 (310) the previous choices are restored from profile words
   `0x36 + p*2 + j` and the gauges from words `0x46 + i` into `gauge[i]`; otherwise those stored
   choices are reset to 0xff, the first two owned cards are preselected and the gauges set to 100. */

void UiCardEquipBuildCardLists(UiCardEquip *self)
{
  SaveProfile *profile;
  int p;
  int k;
  int j;
  int card;
  u32 prev;
  u8 marked;
  u8 count;

  memset(&self->cardIds[0], 0xff, 0x10);
  memset(&self->cardIds[0x10], 0, 0x10);
  if (g_lastScreenTaskId == 0x136) {
    for (p = 0; p < self->bakuganCount; p++) {
      marked = 0;
      for (k = 0; k < 4; k++) {
        profile = SaveGetProfile();
        card = self->bakuganIds[p] * 4 + k - 4;
        if ((profile->data->ownedItems[card / 8] & (1 << (card % 8))) == 0) {
          continue;
        }
        self->cardIds[p * 4 + k] = (u8)card;
        for (j = 0; j < 2; j++) {
          profile = SaveGetProfile();
          prev = 0;
          if (profile->words != NULL) {
            prev = profile->words[0x36 + p * 2 + j];
          }
          if ((u8)prev != 0xff && self->cardIds[p * 4 + k] == (u8)prev) {
            self->cardIds[0x10 + p * 4 + k] |= 1;
            if (marked == 0) {
              self->cardIds[0x10 + p * 4 + k] |= 2;
              marked++;
            }
            break;
          }
        }
      }
    }
    for (k = 0; k < 4; k++) {
      profile = SaveGetProfile();
      prev = 0;
      if (profile->words != NULL) {
        prev = profile->words[0x46 + k];
      }
      self->gauge[k] = (u8)prev;
    }
  }
  else {
    for (p = 0; p < self->bakuganCount; p++) {
      for (j = 0; j < 2; j++) {
        profile = SaveGetProfile();
        if (profile->words != NULL) {
          profile->words[0x36 + p * 2 + j] = 0xff;
        }
      }
      count = 0;
      marked = 0;
      for (k = 0; k < 4; k++) {
        profile = SaveGetProfile();
        card = self->bakuganIds[p] * 4 + k - 4;
        if ((profile->data->ownedItems[card / 8] & (1 << (card % 8))) == 0) {
          continue;
        }
        self->cardIds[p * 4 + k] = (u8)card;
        if (count < 2) {
          self->cardIds[0x10 + p * 4 + k] |= 1;
          if (marked == 0) {
            self->cardIds[0x10 + p * 4 + k] |= 2;
            marked++;
          }
        }
        count++;
      }
    }
    memset(self->gauge, 100, 4);
  }
}
