// bdc 0x0898915c UiCollectionTheaterIsCellUsed
#include "bdc.h"

/* Returns whether cell `cell` of page `page` of `UiCollectionTheater`
   holds a scene slot (cell < 6 and `cell + page*6` < 21). */

bool UiCollectionTheaterIsCellUsed(UiScreen *screen, u8 cell, u8 page)

{
  if (cell < 6) {
    return (uint)cell + (uint)page * 6 < 0x15;
  }
  return false;
}

