// bdc 0x08910f24 UiPauseGetSelectedAction
#include "bdc.h"

/* Returns the action of the selected pause entry (cursor `+0x78`): from `g_pauseActionTable[kind * 7 + cursor]`
   (kind `+0x150`), or from `g_pauseActionTableFlagged[cursor]` when `g_profileFlag0` is set. */

int UiPauseGetSelectedAction(UiPause *self)

{
  if (SaveGetProfileFlag0() == 0) {
    return g_pauseActionTable[self->kind * 7 + self->cursor];
  }
  return g_pauseActionTableFlagged[self->cursor];
}
