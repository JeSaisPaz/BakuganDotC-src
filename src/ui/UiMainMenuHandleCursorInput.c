// bdc 0x089a8bb4 UiMainMenuHandleCursorInput
#include "bdc.h"

/* Handles left/right input on the carousel (pad buttons bit 0x80 = next, 0x20 = previous, or a
   queued move `moveQueued` whose direction is `moveDir`): moves `cursor` by ±1 with wrap-around
   (0..4), stores the old item in `prevCursor`, clears the 12-byte move block (`moveDir`..+0x68b),
   then records the direction (`moveDir` 0 = next, 1 = previous) and `moveSpeed = 8`. Returns 1
   when the cursor moved, 0 otherwise. */

int UiMainMenuHandleCursorInput(UiMainMenu *self)

{
  signed char cur;
  PadState *pad;

  if (self->moveQueued != 0) {
    cur = self->cursor;
    if (self->moveDir == 0) {
      self->prevCursor = cur;
      self->cursor = cur + 1;
      if (!(self->cursor < 5)) {
        self->cursor = 0;
      }
      memset(&self->moveDir, 0, 0xc);
      self->moveDir = 0;
      self->moveSpeed = 8.0f;
      return 1;
    }
    self->prevCursor = cur;
    self->cursor = cur - 1;
    if (self->cursor < 0) {
      self->cursor = 4;
    }
    memset(&self->moveDir, 0, 0xc);
    self->moveDir = 1;
    self->moveSpeed = 8.0f;
    return 1;
  }
  pad = self->base.pad;
  if ((pad->buttons & 0x80) != 0) {
    cur = self->cursor;
    self->cursor = cur + 1;
    self->prevCursor = cur;
    if (!(self->cursor < 5)) {
      self->cursor = 0;
    }
    memset(&self->moveDir, 0, 0xc);
    self->moveDir = 0;
    self->moveSpeed = 8.0f;
    return 1;
  }
  if ((pad->buttons & 0x20) != 0) {
    cur = self->cursor;
    self->cursor = cur - 1;
    self->prevCursor = cur;
    if (self->cursor < 0) {
      self->cursor = 4;
    }
    memset(&self->moveDir, 0, 0xc);
    self->moveDir = 1;
    self->moveSpeed = 8.0f;
    return 1;
  }
  return 0;
}
