// bdc 0x089a86d4 UiMainMenuShouldCloseSharedBg
#include "bdc.h"

/* Returns whether leaving the menu with the current item must close the shared background
   (`UiSharedBgClose`): 1 when cancelled (`+0x672`) or for items 0..4, 0 for out-of-range values.
    */

int UiMainMenuShouldCloseSharedBg(UiMainMenu *self)

{
  u8 cursor;

  if (self->cancelled != 0) {
    return 1;
  }
  cursor = self->cursor;
  if (4 < cursor) {
    return 0;
  }
  if (cursor != 0) {
    if (cursor == 1) {
      return 1;
    }
    if (((cursor != 2) && (cursor != 3)) && (cursor != 4)) {
      return 1;
    }
  }
  return 1;
}
