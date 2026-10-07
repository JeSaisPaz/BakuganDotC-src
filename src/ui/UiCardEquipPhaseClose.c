// bdc 0x0896b570 UiCardEquipPhaseClose
#include "bdc.h"

/* Last phase (4) of `UiCardEquip`: requests its own close (`closeRequested`). */

void UiCardEquipPhaseClose(UiCardEquip *self)

{
  (self->base).closeRequested = '\x01';
  return;
}

