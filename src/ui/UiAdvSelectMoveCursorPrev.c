// bdc 0x0891a17c UiAdvSelectMoveCursorPrev
#include "bdc.h"

/* Horizontal cursor step of the adventure partner-select screen (`UiAdvSelectCtor`, task 376):
   when the pad's repeat byte has bit 0x80 (left) moves the cursor to the previous unlocked candidate
   (wrapping 0 -> 5), otherwise with bit 0x20 (right) to the next one (wrapping 5 -> 0), trying at most
   5 slots. Returns 1 when it moved; 0 when no direction was pressed or every other candidate is
   locked (the cursor is then left unchanged). */

int UiAdvSelectMoveCursorPrev(UiAdvSelect *self)
{
  PadState *pad = self->base.pad;
  s8 start;
  s8 cur;
  int tries;

  if (((s8)pad->repeat & 0x80) != 0) {
    start = self->cursor;
    cur = start;
    tries = 0;
    for (;;) {
      if (cur % 6 != 0) {
        cur = (s8)(cur - 1);
      } else {
        cur = 5;
      }
      tries++;
      if (self->candidates[cur].locked == 0) {
        self->cursor = cur;
        return 1;
      }
      if (tries >= 5) {
        self->cursor = cur;
        self->cursor = start;
        return 0;
      }
    }
  }
  if (((s8)pad->repeat & 0x20) != 0) {
    start = self->cursor;
    cur = start;
    tries = 0;
    do {
      if (cur % 6 != 5) {
        cur = (s8)(cur + 1);
      } else {
        cur = 0;
      }
      tries++;
      if (self->candidates[cur].locked == 0) {
        self->cursor = cur;
        return 1;
      }
    } while (tries < 5);
    self->cursor = cur;
    self->cursor = start;
  }
  return 0;
}
