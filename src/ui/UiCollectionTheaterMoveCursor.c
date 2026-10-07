// bdc 0x08989ad8 UiCollectionTheaterMoveCursor
#include "bdc.h"

/* Moves the cursor `+0x8e0` of `UiCollectionTheater` inside the 3x2 scene
   grid with the D-pad repeat bits, staying on used slots (< 21). Returns 1 when moved, else 0. */

int UiCollectionTheaterMoveCursor(UiScreen *screen)

{
  UiCollectionTheater *self = (UiCollectionTheater *)screen;
  PadState *pad = screen->pad;

  if ((pad->repeat & 0x10) != 0) {
    if (self->cursor >= 3) {
      self->cursor = self->cursor - 3;
      return 1;
    }
  } else if ((pad->repeat & 0x40) != 0) {
    if (self->cursor < 3 && self->page * 6 + self->cursor + 3 < 21) {
      self->cursor = self->cursor + 3;
      return 1;
    }
  } else if ((pad->repeat & 0x80) != 0) {
    if (self->cursor % 3 != 0) {
      self->cursor = self->cursor - 1;
      return 1;
    }
  } else if ((pad->repeat & 0x20) != 0) {
    if (self->cursor % 3 != 2) {
      self->cursor = self->cursor + 1;
      return 1;
    }
  }
  return 0;
}
