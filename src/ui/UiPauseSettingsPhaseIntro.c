// bdc 0x089abe5c UiPauseSettingsPhaseIntro
#include "bdc.h"

/* Phase 1 of `UiPauseSettings`: builds the sprites (`UiPauseSettingsCreateSprites`), waits
   for the fade, plays `main_start.fab` and advances to phase 2 (main). */

void UiPauseSettingsPhaseIntro(UiPauseSettings *self)
{
  int step = self->base.phaseStep;

  if (step > 0) {
    if (step < 2) {
      if (!GfxFaderIsFinished(GfxGetActiveFader())) {
        return;
      }
      UiSharedAnimStart(900.0f, 0.0f, 0.0f, self, (void *)"main_start.fab", 1, 0);
      self->base.phaseStep = self->base.phaseStep + 1;
      return;
    }
  } else if (step == 0) {
    UiPauseSettingsCreateSprites(self);
    self->base.phaseStep = self->base.phaseStep + 1;
    return;
  }
  self->base.phaseStep = 0;
  self->base.phase = self->base.phase + 1;
}
