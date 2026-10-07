// bdc 0x089b1000 UiBattleModeSelectCheckConfirm
#include "bdc.h"

/* Checks the confirm button of `UiBattleModeSelect` (newly pressed bit
   0x4000, Cross, `PadState` `pressed`): returns 0 when it is not pressed, 1 when the entry under
   the cursor `+0x74` is enabled (`+0x579[cursor]`), 2 when it is disabled. */

s32 UiBattleModeSelectCheckConfirm(UiBattleModeSelect *self)

{
  if ((((self->base).pad)->pressed & 0x4000) == 0) {
    return 0;
  }
  if (self->entryEnabled[self->cursor] != '\0') {
    return 1;
  }
  return 2;
}

