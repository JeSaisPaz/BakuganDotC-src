// bdc 0x0898c848 UiCollectionFigureIsCellUsed
#include "bdc.h"

/* Returns whether grid `cell` (0..5) of `page` holds an entry in
   `UiCollectionFigure`: category 0 has 18 entries (3 pages of 6), category
   1 has 2; other categories none. */

bool UiCollectionFigureIsCellUsed(UiCollectionFigure *self, u8 category, u8 cell, u8 page)

{
  uint cellIdx;
  
  cellIdx = (uint)cell;
  if (category == '\0') {
    if (cellIdx < 6) {
      return cellIdx + (uint)page * 6 < 0x12;
    }
  }
  else if ((category < 2) && (cellIdx < 6)) {
    return cellIdx + (uint)page * 6 < 2;
  }
  return false;
}

