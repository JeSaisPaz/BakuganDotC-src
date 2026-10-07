// bdc 0x0898e030 UiCollectionFigureClampCursor
#include "bdc.h"

/* After a page change of `UiCollectionFigure` (category 0), moves the
   cursor `+0xe78` back until it points at an existing entry of the new page (index < count
   `+0xe84`). */

void UiCollectionFigureClampCursor(UiCollectionFigure *self)

{
  int i;
  int pageStart;
  int cursor;
  int index;

  if (self->category == 0) {
    i = 0;
    pageStart = self->page * 6;
    if (0 < pageStart) {
      cursor = self->cursor;
      index = cursor + pageStart;
      do {
        i = i + 1;
        if (index < (int)(uint)self->entryCount) {
          self->cursor = (s8)cursor;
          return;
        }
        cursor = cursor - 1;
        index = index - 1;
      } while (i < pageStart);
    }
  }
  return;
}
