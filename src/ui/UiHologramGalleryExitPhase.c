// bdc 0x0891ed10 UiHologramGalleryExitPhase
#include "bdc.h"

/* Phase 4 of the hologram gallery: stops BGM channel 1 (`SndBgmQueueStop`), waits for the player
   to stop, releases the voice pac and sets `closeRequested`. */

void UiHologramGalleryExitPhase(UiHologramGallery *self)
{
  int step = self->base.phaseStep;

  if (step < 1) {
    if (step >= 0) {
      SndBgmCancelChannel(1);
      SndBgmQueueStop(0.1f, 1);
      self->base.phaseStep = self->base.phaseStep + 1;
    }
  } else if (step < 2) {
    if (SndBgmPlayerExists(1)) {
      if (!SndBgmPlayerIsStopped(SndBgmPlayerGet(1))) {
        return;
      }
      step = self->base.phaseStep;
    }
    self->base.phaseStep = step + 1;
  } else if (step < 3 && !SndVoicePacIsIdle()) {
    SndVoicePacRelease();
    self->base.closeRequested = 1;
    self->base.phaseStep = self->base.phaseStep + 1;
  }
}
