// bdc 0x0893af3c UiUnlockResultResetBlink
#include "bdc.h"

/* Resets the blink state of sprite 5 of `UiUnlockResult` (`+0x784` t = 0,
   start alpha `+0x788` = 1, direction `+0x78c` = 0). */

void UiUnlockResultResetBlink(UiUnlockResult *self)

{
  memset(&self->blinkT,0,0xc);
  self->blinkBase = 1.0f;
  return;
}

