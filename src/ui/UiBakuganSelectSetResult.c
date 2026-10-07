// bdc 0x0892cdec UiBakuganSelectSetResult
#include "bdc.h"

/* Sets the menu result of the Bakugan select screen (`UiBakuganSelectCtor`, task 371; cursor
   `+0x74`, current entry `+0x75`, owned list `+0x1ba4` with 0xc-byte entries): 1 (changed) unless
   the cancel flag is set, then 0 (`UiSetMenuResult`). */

void UiBakuganSelectSetResult(UiBakuganSelect *self)

{
  if (self->cancelled == '\0') {
    UiSetMenuResult(&self->base,1);
    return;
  }
  UiSetMenuResult(&self->base,0);
  return;
}

