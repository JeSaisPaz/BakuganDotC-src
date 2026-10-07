// bdc 0x0898acdc UiCollectionFigureGetCat1EntryId
#include "bdc.h"

/* Returns entry `index` of the figure id tables for category 1 of
   `UiCollectionFigure`: the 33-byte table `g_uiCollectionFigureCat1Ids` (`special` = 0) or
   the two-entry list {2, 10}. */

u8 UiCollectionFigureGetCat1EntryId(UiCollectionFigure *self, u8 special, u8 index)

{
  u8 result;
  u8 ids [36];
  u8 special_ids [4];
  
  memcpy(ids,g_uiCollectionFigureCat1Ids,0x21);
  special_ids[0] = '\x02';
  special_ids[1] = 10;
  if (special == '\0') {
    result = ids[index];
  }
  else {
    result = special_ids[index];
  }
  return result;
}

