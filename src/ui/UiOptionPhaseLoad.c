// bdc 0x08970360 UiOptionPhaseLoad
#include "bdc.h"

/* Phase 1 of `UiOption`: builds the sprites (`UiOptionCreateSprites`) in step 0,
   then refreshes the profile flag (`SaveRefreshProfileFlag0`) and goes to phase 2
   (`UiOptionMainPhase`) or, with `SaveGetProfileFlag0` set, to phase 4
   (`UiOptionNetMainPhase`). */

void UiOptionPhaseLoad(UiOption *self)

{
  if ((self->base).phaseStep == 0) {
    UiOptionCreateSprites(self);
    (self->base).phaseStep = (self->base).phaseStep + 1;
  }
  else {
    (self->base).phase = 2;
    (self->base).phaseStep = 0;
    SaveRefreshProfileFlag0();
    if (SaveGetProfileFlag0() != 0) {
      (self->base).phase = 4;
      self->netDirty = '\0';
    }
  }
  return;
}

