// bdc 0x0891cfa8 UiHologramGalleryFadeInPhase
#include "bdc.h"

/* Phase 1 of the hologram gallery: waits for the fader to finish, then advances. */

void UiHologramGalleryFadeInPhase(UiHologramGallery *self)
{
  int step = self->base.phaseStep;

  if (step > 0) {
    if (step < 2) {
      self->base.phaseStep = step + 1;
      return;
    }
  } else if (step >= 0) {
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      self->base.phaseStep = self->base.phaseStep + 1;
    }
    return;
  }
  step = self->base.phase;
  self->base.phaseStep = 0;
  self->base.phase = step + 1;
}
