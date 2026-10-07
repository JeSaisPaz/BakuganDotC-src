// bdc 0x08918468 UiAdvSelectSetResultNone
#include "bdc.h"

/* Sets the menu result of the adventure partner-select screen (`UiAdvSelectCtor`, task 376;
   cursor `+0x74`, candidates `+0x8a0` as 4-byte `{id, partner, locked, ?}` slots) to 0
   (`UiSetMenuResult`); both branches of its flag test do the same. */

void UiAdvSelectSetResultNone(UiAdvSelect *self)

{
  if (self->flag898 == '\0') {
    UiSetMenuResult(&self->base,0);
    return;
  }
  UiSetMenuResult(&self->base,0);
  return;
}

