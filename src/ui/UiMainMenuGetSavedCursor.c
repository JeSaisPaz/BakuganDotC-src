// bdc 0x089a5d30 UiMainMenuGetSavedCursor
#include "bdc.h"

/* Returns the main-menu item saved in profile word 0x17 (0..4), or 0 when the stored value is out
   of range. */

int UiMainMenuGetSavedCursor(void)

{
  int cursor = 0;

  if ((int)SaveProfileGetWord(SaveGetProfile(), 0x17) < 5) {
    cursor = (char)SaveProfileGetWord(SaveGetProfile(), 0x17);
  }
  return cursor;
}
