// bdc 0x0897a674 UiCollectionSphereIsCellUsed
#include "bdc.h"

/* Returns whether cell `cell` of page `page` exists in category `category` of
   `UiCollectionSphere` (0: fewer than 19 entries, 1: fewer than 11, 2: six
   cells on model pages, only cell 0 on special pages). */

bool UiCollectionSphereIsCellUsed(UiCollectionSphere *self, u8 category, u8 cell, u8 page)

{
  if (category == 0) {
    if (cell < 6) {
      return cell + page * 6 < 0x13;
    }
  }
  else if (category < 2) {
    if (cell < 6) {
      return cell + page * 6 < 0xb;
    }
  }
  else if (category < 3) {
    if (page % 3 != 0) {
      return cell == 0;
    }
    if (cell < 6) {
      return true;
    }
  }
  return false;
}

