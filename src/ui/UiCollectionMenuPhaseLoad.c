// bdc 0x089743dc UiCollectionMenuPhaseLoad
#include "bdc.h"

/* Phase 1 of `UiCollectionMenu`: creates the sprites, camera and item-box
   model (`UiCollectionMenuCreateSprites`, `UiCollectionMenuCreateCamera`,
   `UiCollectionMenuLoadItemBox`) with all lights off, waits for the fade, starts
   `"main_start.fab"` and advances to the main phase. */

void UiCollectionMenuPhaseLoad(UiCollectionMenu *self)

{
  int step;

  step = self->base.phaseStep;
  if (step > 0) {
    if (step < 2) {
      if (!GfxFaderIsFinished(GfxGetActiveFader())) {
        return;
      }
      UiSharedAnimStart(15.0f, 0.0f, 0.0f, self, (void *)"main_start.fab", 1, 0);
      self->base.phaseStep = self->base.phaseStep + 1;
      return;
    }
  } else if (step >= 0) {
    UiCollectionMenuCreateSprites(self);
    UiCollectionMenuCreateCamera(self);
    UiCollectionMenuLoadItemBox(self);
    UiCollectionMenuClearItemBoxLights(self);
    self->base.phaseStep = self->base.phaseStep + 1;
    return;
  }
  self->base.phaseStep = 0;
  self->base.phase = self->base.phase + 1;
}
