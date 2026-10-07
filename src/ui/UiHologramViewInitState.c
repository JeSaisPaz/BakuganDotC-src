// bdc 0x08928e84 UiHologramViewInitState
#include "bdc.h"

/* Clears the state of the hologram detail view (`UiHologramViewCtor`, task 392; view kind
   `kind`): blink record `blinkOn`, `entryIndex`, text block `printer..text`, flags
   `helpDone`/`helpStep`, fade record `screenFade`; then loads the kind table
   (`UiHologramViewLoadKindTable` for `kind`). */

void UiHologramViewInitState(UiHologramView *self)
{
  memset(&self->blinkOn, 0, 0x10);
  self->entryIndex = 0;
  UiHologramViewLoadKindTable(self, self->kind);
  memset(&self->printer, 0, 0x224);
  self->helpDone = 0;
  self->helpStep = 0;
  memset(self->screenFade, 0, 0x10);
}
