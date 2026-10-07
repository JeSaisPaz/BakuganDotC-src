// bdc 0x0891a858 UiAdvSelectResetDecide
#include "bdc.h"

/* Clears the decide-sequence record `+0x90c` of the adventure partner-select screen
   (`UiAdvSelectCtor`, task 376; cursor `+0x74`, candidates `+0x8a0` as 4-byte `{id, partner,
   locked, ?}` slots) (see `UiAdvSelectRunDecide`). */

void UiAdvSelectResetDecide(UiAdvSelect *self)

{
  memset(&self->decideStep,0,4);
  return;
}

