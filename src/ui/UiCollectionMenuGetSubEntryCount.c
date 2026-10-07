// bdc 0x08975748 UiCollectionMenuGetSubEntryCount
#include "bdc.h"

/* Returns the number of sub-page entries for main entry `entry` of
   `UiCollectionMenu`: 3 for entry 0, 0 for 1, 2 for 2, 0 for 3/4. */

int UiCollectionMenuGetSubEntryCount(UiCollectionMenu *self, u8 entry)

{
  if (entry < 5) {
    if (entry == '\x01') {
      return 0;
    }
    if (entry == '\x02') {
      return 2;
    }
    if ((entry != '\x03') && (entry != '\x04')) {
      return 3;
    }
  }
  return 0;
}

