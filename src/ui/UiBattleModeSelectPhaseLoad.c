// bdc 0x089b009c UiBattleModeSelectPhaseLoad
#include "bdc.h"

/* Phase 1 of `UiBattleModeSelect`: in step 0 builds the sprites
   (`UiBattleModeSelectCreateSprites`) and moves to step 1; on the next call resets the step to
   0 and advances to phase 2. */
void UiBattleModeSelectPhaseLoad(UiBattleModeSelect *self)
{
    if (self->base.phaseStep == 0) {
        UiBattleModeSelectCreateSprites(self);
        self->base.phaseStep++;
    } else {
        self->base.phaseStep = 0;
        self->base.phase++;
    }
}
