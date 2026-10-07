// bdc 0x089b1730 UiBattleModeSelectApplyResult
#include "bdc.h"

/* Stores the menu result of `UiBattleModeSelect` when it closes
   (`UiSetMenuResult`): cancelled → 0; entry 0 → 1 and profile word 7 cleared
   (`SaveProfileSetWord`); entry 1 → 2. */

void UiBattleModeSelectApplyResult(UiBattleModeSelect *self)
{
  s32 cursor;

  if (self->cancelled == 0) {
    cursor = self->cursor;
    if (cursor <= 0) {
      if (cursor >= 0) {
        UiSetMenuResult(&self->base, 1);
        SaveProfileSetWord(SaveGetProfile(), 7, 0);
        return;
      }
    } else if (cursor < 2) {
      UiSetMenuResult(&self->base, 2);
      return;
    }
  } else {
    UiSetMenuResult(&self->base, 0);
  }
}
