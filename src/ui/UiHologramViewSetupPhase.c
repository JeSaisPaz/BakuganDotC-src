// bdc 0x089295dc UiHologramViewSetupPhase
#include "bdc.h"

/* Phase 0 of the hologram view: builds the screen (`UiHologramViewCreateSprites`), waits a frame,
   then advances. */
void UiHologramViewSetupPhase(UiHologramView *self)
{
    if (self->base.phaseStep == 0) {
        UiHologramViewCreateSprites(self);
        self->base.phaseStep++;
    } else {
        self->base.phaseStep = 0;
        self->base.phase++;
    }
}
