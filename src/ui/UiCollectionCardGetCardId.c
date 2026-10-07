// bdc 0x0898217c UiCollectionCardGetCardId
#include "bdc.h"

/* Returns entry `index` of the card id tables of `UiCollectionCard`: the
   21-byte table `0x08ac39c8` (`second` = 0) or the 20-byte table `0x08ac39dd`. */

u8 UiCollectionCardGetCardId(UiCollectionCard *self, u8 second, u8 index)

{
  u8 result;
  u8 idsA [24];
  u8 idsB [20];
  
  memcpy(idsA,g_collectionCardIdsA,0x15);
  memcpy(idsB,g_collectionCardIdsB,0x14);
  if (second == '\0') {
    result = idsA[index];
  }
  else {
    result = idsB[index];
  }
  return result;
}

