// bdc 0x08992ed0 UiUnlockCodeSetMessage
#include "bdc.h"

/* Selects message `msg` of the `"DWSpecialUnlock"` table for the next dialog of
   `UiUnlockCode` (`+0x10d`) and resets the dialog step `+0x10c`; shown by
   `UiUnlockCodeShowMessage`. */

void UiUnlockCodeSetMessage(UiUnlockCode *self, u8 msg)

{
  memset(&self->messageStep,0,2);
  self->messageId = msg;
  return;
}

