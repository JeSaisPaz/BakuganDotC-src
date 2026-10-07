// bdc 0x0898ac74 UiCollectionFigureGetCat0EntryId
#include "bdc.h"

/* Returns entry `index` of the figure id tables for category 0 of
   `UiCollectionFigure`: the 33-byte table `0x08ac3a84` (`special` = 0) or
   the 18-byte table `0x08ac3aa5`. */

u8 UiCollectionFigureGetCat0EntryId(UiCollectionFigure *self, u8 special, u8 index)

{
  u8 result;
  u8 ids [36];
  u8 special_ids [20];

  memcpy(ids,g_uiCollectionFigureCat0IdsNormal,0x21);
  memcpy(special_ids,g_uiCollectionFigureCat0IdsSpecial,0x12);
  if (special == '\0') {
    result = ids[index];
  }
  else {
    result = special_ids[index];
  }
  return result;
}
