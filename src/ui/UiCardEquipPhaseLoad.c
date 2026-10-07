// bdc 0x08969f48 UiCardEquipPhaseLoad
#include "bdc.h"

/* Phase 1 of `UiCardEquip`: builds the sprites and text
   (`UiCardEquipCreateSprites`, `UiCardEquipInitLayoutTable`) in step 0 and advances to phase 2
   on the next frame. */

void UiCardEquipPhaseLoad(UiCardEquip *self)

{
  if ((self->base).phaseStep == 0) {
    UiCardEquipCreateSprites(self);
    UiCardEquipInitLayoutTable(self);
    (self->base).phaseStep = (self->base).phaseStep + 1;
  }
  else {
    (self->base).phaseStep = 0;
    (self->base).phase = (self->base).phase + 1;
  }
  return;
}

