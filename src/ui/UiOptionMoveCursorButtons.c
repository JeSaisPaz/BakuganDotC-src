// bdc 0x089718d0 UiOptionMoveCursorButtons
#include "bdc.h"

/* On the button row of `UiOption`, Left/Right switches between OK (4) and Defaults
   (5). Returns 1 when moved, 0 otherwise. */

int UiOptionMoveCursorButtons(UiOption *self)
{
  PadState *pad;
  u8 next;

  if (((char)self->cursor > 3) &&
      (pad = self->base.pad, (((int)(char)pad->repeat & 0x80U) != 0) || ((pad->repeat & 0x20) != 0))) {
    next = 4;
    if (self->cursor == 4) {
      next = 5;
    }
    self->cursor = next;
    return 1;
  }
  return 0;
}
