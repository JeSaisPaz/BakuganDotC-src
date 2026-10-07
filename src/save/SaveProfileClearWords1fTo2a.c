// bdc 0x0880d764 SaveProfileClearWords1fTo2a
#include "bdc.h"

/* Zeroes profile words 0x1f..0x2a (12 consecutive words) through `SaveProfileSetWord`. Called by
   `UiMainMenuCtor` each time the main menu opens, so these words hold per-session state. */

void SaveProfileClearWords1fTo2a(SaveProfile *self)

{
  int index;
  
  index = 0x1f;
  do {
    SaveProfileSetWord(self,index,0);
    index = index + 1;
  } while (index < 0x2b);
  return;
}

