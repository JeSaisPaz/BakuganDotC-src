// bdc 0x08982eec UiCollectionCardPhaseLoad
#include "bdc.h"

/* Phase 1 of `UiCollectionCard`: creates the sprites
   (`UiCollectionCardCreateSprites`) in step 0 and advances to the main phase on the next frame. */
void UiCollectionCardPhaseLoad(UiCollectionCard *self)
{
    if (self->base.phaseStep == 0) {
        UiCollectionCardCreateSprites(self);
        self->base.phaseStep++;
    } else {
        self->base.phaseStep = 0;
        self->base.phase++;
    }
}
