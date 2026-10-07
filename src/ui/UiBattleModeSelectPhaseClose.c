// bdc 0x089b1ae4 UiBattleModeSelectPhaseClose
#include "bdc.h"

/* Last phase (3) of `UiBattleModeSelect`: requests its own close
   (`closeRequested`). */

void UiBattleModeSelectPhaseClose(UiBattleModeSelect *self)

{
  (self->base).closeRequested = '\x01';
  return;
}

