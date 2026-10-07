// bdc 0x0891927c UiAdvSelectStartBlinkA
#include "bdc.h"

/* Arms the alpha blink of sprite 0x25 of the adventure partner-select screen (`UiAdvSelectCtor`,
   task 376; cursor `+0x74`, candidates `+0x8a0` as 4-byte `{id, partner, locked, ?}` slots): record
   `+0x8b8 = {on, phase 0, t 0, base 1.0}`. */

void UiAdvSelectStartBlinkA(UiAdvSelect *self, u8 on)

{
  memset(&self->blinkAOn,0,0xc);
  self->blinkAOn = on;
  self->blinkABase = 1.0;
  return;
}

