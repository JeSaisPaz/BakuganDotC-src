// bdc 0x08989f3c UiCollectionTheaterClampCursorToPage
#include "bdc.h"

/* After a page change in `UiCollectionTheater`, moves the cursor back
   onto the last used slot of the new page. */

void UiCollectionTheaterClampCursorToPage(UiScreen *screen)

{
  UiCollectionTheater *self = (UiCollectionTheater *)screen;
  int count = self->page * 6;
  int i;
  int cursor;

  for (i = 0; i < count; i++) {
    cursor = self->cursor;
    if (cursor + count - i < 0x15) {
      self->cursor = (s8)(cursor - i);
      return;
    }
  }
}
