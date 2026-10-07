// bdc 0x0893af08 UiUnlockResultSetHelpTextVisible
#include "bdc.h"

/* Sets the help-text alpha `+0x744` of `UiUnlockResult` to 1 (`visible`) or 0
   and marks it dirty (`+0x779`). */

void UiUnlockResultSetHelpTextVisible(UiUnlockResult *self, u8 visible)

{
  if (visible != '\0') {
    self->textAlpha[1] = 1.0f;
    self->textVisible[1] = '\x01';
    return;
  }
  self->textAlpha[1] = 0.0f;
  self->textVisible[1] = '\x01';
  return;
}

