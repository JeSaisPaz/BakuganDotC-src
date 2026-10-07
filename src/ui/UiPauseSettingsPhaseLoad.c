// bdc 0x089abbec UiPauseSettingsPhaseLoad
#include "bdc.h"

/* Phase 0 of `UiPauseSettings`: step 1 starts `main_bg.fab` (`UiSharedAnimStart`),
   step 2 starts a 16-frame fade-in and advances to phase 1. */

void UiPauseSettingsPhaseLoad(UiPauseSettings *self)
{
  int step = self->base.phaseStep;

  if (step > 0) {
    if (step < 2) {
      UiSharedAnimStart(800.0f, 0.0f, 0.0f, self, (void *)"main_bg.fab", 0, 0);
      self->base.phaseStep = self->base.phaseStep + 1;
      return;
    }
  } else if (step >= 0) {
    self->base.phaseStep = step + 1;
    return;
  }
  GfxFader *fader = GfxGetActiveFader();
  fader->start[0] = 0.0f;
  fader->start[1] = 0.0f;
  fader->start[2] = 0.0f;
  fader->start[3] = 1.0f;
  fader = GfxGetActiveFader();
  fader->end[0] = 0.0f;
  fader->end[1] = 0.0f;
  fader->end[2] = 0.0f;
  fader->end[3] = 0.0f;
  GfxFaderStart(GfxGetActiveFader(), 0x10);
  self->base.phaseStep = 0;
  self->base.phase = self->base.phase + 1;
}
