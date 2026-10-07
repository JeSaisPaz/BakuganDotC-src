// bdc 0x089821e4 UiCollectionCardBuildCardList
#include "bdc.h"

/* Builds the owned-card list of `UiCollectionCard`: fills `slots` (80
   slots, 4 per page) with 0xff and clears `isNew`, then for each of the 20 card ids
   (`UiCollectionCardGetCardId`, table 1) and its four variants stores the card number in its
   slot when the profile's owned bit is set (`newItemGroups`, data `+0x5d3`) and flags it new
   when its seen bit (`newItemGroups[0x20..]`, data `+0x5f3`) is clear. */

void UiCollectionCardBuildCardList(UiCollectionCard *self)
{
  int i;
  int j;
  int base;
  int card;

  memset(self->slots, 0xff, 0x50);
  memset(self->isNew, 0, 0x50);
  for (i = 0; i < 20; i++) {
    base = (u16)(UiCollectionCardGetCardId(self, 1, (u8)i) - 1) * 4;
    for (j = 0; j < 4; j++) {
      card = base + j;
      if ((u8)(SaveGetProfile()->data->newItemGroups[card / 8] & (1 << (card % 8))) != 0) {
        self->slots[i * 4 + j] = (u8)card;
        if ((u8)(SaveGetProfile()->data->newItemGroups[0x20 + card / 8] & (1 << (card % 8))) == 0) {
          self->isNew[i * 4 + j] = 1;
        }
      }
    }
  }
}
