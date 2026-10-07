// bdc 0x089703c8 UiOptionPhaseClose
#include "bdc.h"

/* Phase 3 of `UiOption`: requests the screen's removal (`closeRequested`, `+0x4c` =
   1). */

void UiOptionPhaseClose(UiOption *self)

{
  (self->base).closeRequested = '\x01';
  return;
}

