// bdc 0x08971760 UiOptionMoveCursorVertical
#include "bdc.h"

/* Moves the row cursor of `UiOption` with Up/Down repeat: through value rows 0..3 to
   the button row (4) and back, skipping row 0 when `SaveGetProfileFlag0` disables it. Returns 1
   when moved, 0 when neither Up nor Down repeated. */

int UiOptionMoveCursorVertical(UiOption *self)
{
  s8 cursor;
  PadState *pad;

  if (SaveGetProfileFlag0() == 0) {
    pad = self->base.pad;
    if (pad->repeat & 0x10) {
      cursor = (s8)self->cursor;
      if (cursor == 0) {
        self->cursor = 4;
      }
      else if (cursor < 4) {
        self->cursor = (s8)(cursor - 1);
      }
      else {
        self->cursor = 3;
      }
      return 1;
    }
    if (pad->repeat & 0x40) {
      cursor = (s8)self->cursor;
      if (cursor != 5 && cursor < 4) {
        self->cursor = (s8)(cursor + 1);
      }
      else {
        self->cursor = 0;
      }
      return 1;
    }
  }
  else {
    pad = self->base.pad;
    if (pad->repeat & 0x10) {
      cursor = (s8)self->cursor;
      if (cursor < 2) {
        self->cursor = 4;
      }
      else if (cursor < 4) {
        self->cursor = (s8)(cursor - 1);
      }
      else {
        self->cursor = 3;
      }
      return 1;
    }
    if (pad->repeat & 0x40) {
      cursor = (s8)self->cursor;
      if (cursor != 5 && cursor < 4) {
        self->cursor = (s8)(cursor + 1);
      }
      else {
        self->cursor = 1;
      }
      return 1;
    }
  }
  return 0;
}
