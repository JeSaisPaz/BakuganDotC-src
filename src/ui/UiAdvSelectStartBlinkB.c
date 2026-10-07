// bdc 0x089193c4 UiAdvSelectStartBlinkB
#include "bdc.h"

/* Arms the alpha blink of sprite 0x19 of the adventure partner-select screen (`UiAdvSelectCtor`,
   task 376; cursor `+0x74`, candidates `+0x8a0` as 4-byte `{id, partner, locked, ?}` slots) (record
   `+0x8c4`, base 1.0). */

void UiAdvSelectStartBlinkB(UiAdvSelect *self, u8 on)

{
  memset(&self->blinkBOn,0,0xc);
  self->blinkBOn = on;
  self->blinkBBase = 1.0;
  return;
}

