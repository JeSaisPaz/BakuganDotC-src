// bdc 0x08932ea4 UiGauntletSetupSetupPhase
#include "bdc.h"

/* Phase 0 of the gauntlet setup screen: builds the screen (`UiGauntletSetupInitSprites`, `UiGauntletSetupCreateViews`), then
   starts a 16-frame fade-in and advances. */

void UiGauntletSetupSetupPhase(UiGauntletSetup *self)

{
  GfxFader *fader;

  if ((self->base).phaseStep == 0) {
    UiGauntletSetupInitSprites(self);
    UiGauntletSetupCreateViews(self);
    (self->base).phaseStep = (self->base).phaseStep + 1;
  }
  else {
    fader = GfxGetActiveFader();
    fader->start[0] = 0.0f;
    fader->start[1] = 0.0f;
    fader->start[2] = 0.0f;
    fader->start[3] = 1.0f;
    fader = GfxGetActiveFader();
    fader->end[0] = 0.0f;
    fader->end[1] = 0.0f;
    fader->end[2] = 0.0f;
    fader->end[3] = 0.0f;
    fader = GfxGetActiveFader();
    GfxFaderStart(fader, 0x10);
    (self->base).phaseStep = 0;
    (self->base).phase = (self->base).phase + 1;
  }
}
