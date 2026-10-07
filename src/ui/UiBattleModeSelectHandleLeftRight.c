// bdc 0x089b107c UiBattleModeSelectHandleLeftRight
#include "bdc.h"

/* Moves the cursor of `UiBattleModeSelect` between its two entries: takes
   a queued switch (`switchQueued`, direction `switchDir`) or held Left (D-pad bit 0x80: cursor + 1,
   direction 0) / Right (0x20: cursor - 1, direction 1), wrapping in 0..1, keeps the previous entry
   in `prevCursor`, resets the 12-byte switch state at `switchDir` and sets `switchFrames = 8.0`.
   Returns true when the cursor moved, false when neither a switch was queued nor Left/Right held. */

bool UiBattleModeSelectHandleLeftRight(UiBattleModeSelect *self)
{
  PadState *pad;
  s8 cursor;

  if (self->switchQueued != 0) {
    cursor = self->cursor;
    if (self->switchDir == 0) {
      self->prevCursor = cursor;
      self->cursor = cursor + 1;
      if (self->cursor >= 2) {
        self->cursor = 0;
      }
      memset(&self->switchDir, 0, 0xc);
      self->switchDir = 0;
      self->switchFrames = 8.0f;
      return true;
    }
    self->prevCursor = cursor;
    self->cursor = cursor - 1;
    if (self->cursor < 0) {
      self->cursor = 1;
    }
    memset(&self->switchDir, 0, 0xc);
    self->switchDir = 1;
    self->switchFrames = 8.0f;
    return true;
  }

  pad = self->base.pad;
  if (pad->buttons & 0x80) { /* Left */
    cursor = self->cursor;
    self->cursor = cursor + 1;
    self->prevCursor = cursor;
    if (self->cursor >= 2) {
      self->cursor = 0;
    }
    memset(&self->switchDir, 0, 0xc);
    self->switchDir = 0;
    self->switchFrames = 8.0f;
    return true;
  }
  if (pad->buttons & 0x20) { /* Right */
    cursor = self->cursor;
    self->cursor = cursor - 1;
    self->prevCursor = cursor;
    if (self->cursor < 0) {
      self->cursor = 1;
    }
    memset(&self->switchDir, 0, 0xc);
    self->switchDir = 1;
    self->switchFrames = 8.0f;
    return true;
  }
  return false;
}
