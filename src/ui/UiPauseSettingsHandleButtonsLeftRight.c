// bdc 0x089ad3e4 UiPauseSettingsHandleButtonsLeftRight
#include "bdc.h"

/* On the button row (item ≥ 4) moves the cursor left/right among the `+0xbb3 - 4` buttons with
   wrap-around; returns 1 when it moved. */

int UiPauseSettingsHandleButtonsLeftRight(UiPauseSettings *self)

{
  s8 cur;
  PadState *pad;

  cur = self->cursor;
  if (cur >= 4) {
    pad = self->base.pad;
    if (((s8)pad->repeat & 0x80) != 0) {
      self->cursor = cur - 1;
      if (self->cursor == 3) {
        self->cursor = self->itemCount - 1;
      }
      return 1;
    }
    if ((pad->repeat & 0x20) != 0) {
      self->cursor = cur + 1;
      if (self->cursor == self->itemCount) {
        self->cursor = 4;
      }
      return 1;
    }
  }
  return 0;
}

