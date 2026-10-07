// bdc 0x0891950c UiAdvSelectStartArrowFlash
#include "bdc.h"

/* Arms the arrow flashing of the adventure partner-select screen (`UiAdvSelectCtor`, task 376;
   cursor `+0x74`, candidates `+0x8a0` as 4-byte `{id, partner, locked, ?}` slots): clears the
   record `+0x8d0` and sets its on byte. */

void UiAdvSelectStartArrowFlash(UiAdvSelect *self, u8 on)

{
  memset(&self->arrowFlashOn,0,0xc);
  self->arrowFlashOn = on;
  return;
}

