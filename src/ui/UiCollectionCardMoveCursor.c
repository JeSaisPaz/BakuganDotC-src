// bdc 0x08984748 UiCollectionCardMoveCursor
#include "bdc.h"

/* Moves the cursor `+0xbcc` of `UiCollectionCard` inside the 2x2 card grid
   with the D-pad repeat bits. Returns 1 when moved. */

int UiCollectionCardMoveCursor(UiCollectionCard *self)

{
  PadState *pad = self->base.pad;

  if (pad->repeat & 0x10) {
    if (self->cursor >= 2) {
      self->cursor -= 2;
      return 1;
    }
  } else if (pad->repeat & 0x40) {
    if (self->cursor < 2) {
      self->cursor += 2;
      return 1;
    }
  } else if (pad->repeat & 0x80) {
    if (self->cursor % 2 != 0) {
      self->cursor -= 1;
      return 1;
    }
  } else if (pad->repeat & 0x20) {
    if (self->cursor % 2 != 1) {
      self->cursor += 1;
      return 1;
    }
  }
  return 0;
}
