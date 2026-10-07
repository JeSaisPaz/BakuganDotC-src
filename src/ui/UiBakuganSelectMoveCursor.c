// bdc 0x0892eb90 UiBakuganSelectMoveCursor
#include "bdc.h"

/* Moves the grid cursor of the Bakugan select screen (`UiBakuganSelectCtor`), a 2x10 grid, with
   the pad repeat bits (low byte of `repeat`): UP (0x10) and DOWN (0x40) both toggle the row
   (cursor < 10 ? +10 : -10); LEFT (0x80) steps left, wrapping column 0 to 9 in the same row; RIGHT
   (0x20) steps right, wrapping column 9 to 0. Only the first matching bit is handled. Returns 1
   when a direction was pressed, else 0. */

int UiBakuganSelectMoveCursor(UiBakuganSelect *self)
{
  PadState *pad = self->base.pad;
  s8 cursor;

  if (pad->repeat & 0x10) {
    cursor = self->cursor;
    if (cursor < 10) {
      self->cursor = (s8)(cursor + 10);
      return 1;
    }
    self->cursor = (s8)(cursor - 10);
    return 1;
  }
  if (pad->repeat & 0x40) {
    cursor = self->cursor;
    if (cursor < 10) {
      self->cursor = (s8)(cursor + 10);
      return 1;
    }
    self->cursor = (s8)(cursor - 10);
    return 1;
  }
  if (pad->repeat & 0x80) {
    cursor = self->cursor;
    if (cursor % 10 == 0) {
      self->cursor = (s8)(cursor + 9);
      return 1;
    }
    self->cursor = (s8)(cursor - 1);
    return 1;
  }
  if (pad->repeat & 0x20) {
    cursor = self->cursor;
    if (cursor % 10 == 9) {
      self->cursor = (s8)(cursor - 9);
      return 1;
    }
    self->cursor = (s8)(cursor + 1);
    return 1;
  }
  return 0;
}
