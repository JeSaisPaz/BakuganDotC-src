// bdc 0x0892cd0c UiBakuganSelectPhaseLoad
#include "bdc.h"

/* Phase 0 of `UiBakuganSelect`: builds the screen (`UiBakuganSelectCreateSprites`,
   `UiBakuganSelectCreateCamera`) in step 0, then starts a 16-frame fade-in and advances to phase 1. */

void UiBakuganSelectPhaseLoad(UiBakuganSelect *self)

{
  GfxFader *fader;
  
  if ((self->base).phaseStep == 0) {
    UiBakuganSelectCreateSprites(self);
    UiBakuganSelectCreateCamera(self);
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
    GfxFaderStart(fader,0x10);
    (self->base).phaseStep = 0;
    (self->base).phase = (self->base).phase + 1;
  }
  return;
}

