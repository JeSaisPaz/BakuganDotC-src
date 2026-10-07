// bdc 0x0898dd78 UiCollectionFigureMoveCursor
#include "bdc.h"

/* Moves the cursor `+0xe78` of `UiCollectionFigure` inside the 3x2 grid
   from the pad auto-repeat mask (up/down ±3, left/right ±1), staying within the `+0xe84` entries
   of the current page `+0xe79` (down on a short last page jumps to its last entry). Only for
   categories 0/1 (`+0xe7d`). Returns 1 when the cursor moved. */

int UiCollectionFigureMoveCursor(UiCollectionFigure *self)
{
  PadState *pad;
  int cursor;
  int count;

  if (self->category < 0 || self->category >= 2) {
    return 0;
  }
  pad = self->base.pad;
  if (pad->repeat & 0x10) {           /* up */
    if (self->cursor >= 3) {
      self->cursor = self->cursor - 3;
      return 1;
    }
  } else if (pad->repeat & 0x40) {    /* down */
    cursor = self->cursor;
    if (cursor < 3) {
      count = self->entryCount;
      if (self->page * 6 + cursor + 3 < count) {
        self->cursor = cursor + 3;
        return 1;
      }
      if (count % 6 > 3) {
        self->cursor = count % 6 - 1;
        return 1;
      }
    }
  } else if (pad->repeat & 0x80) {    /* left */
    cursor = self->cursor;
    if (cursor % 3 != 0) {
      self->cursor = cursor - 1;
      return 1;
    }
  } else if (pad->repeat & 0x20) {    /* right */
    cursor = self->cursor;
    if (cursor % 3 != 2 && cursor + self->page * 6 + 1 < self->entryCount) {
      self->cursor = cursor + 1;
      return 1;
    }
  }
  return 0;
}
