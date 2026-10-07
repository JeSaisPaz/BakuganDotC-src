// bdc 0x089182fc UiAdvSelectSetupPhase
#include "bdc.h"

/* Phase 0 of the adventure select screen, one step per call (`phaseStep`): step 0 builds sprites and
   models (`UiAdvSelectInitSprites`, `UiAdvSelectCreateCamera`); step 1 starts the `"main_bg.fab"`
   background animation (`UiSharedAnimStart`, depth 10, slot 0, no loop); any other step (2+ or
   negative) sets the active fader to black → transparent, starts a 16-frame fade-in, queues BGM 1 on
   channel 0 (`SndBgmQueuePlay`, looping), resets `phaseStep` and advances `phase`. */

void UiAdvSelectSetupPhase(UiAdvSelect *self)

{
  GfxFader *fader;
  int step;

  step = self->base.phaseStep;
  if (step > 0) {
    if (step < 2) {
      UiSharedAnimStart(10.0f, 0.0f, 0.0f, self, (void *)"main_bg.fab", 0, 0);
      self->base.phaseStep = self->base.phaseStep + 1;
      return;
    }
  }
  else if (step >= 0) {
    UiAdvSelectInitSprites(self);
    UiAdvSelectCreateCamera(self);
    self->base.phaseStep = self->base.phaseStep + 1;
    return;
  }
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
  SndBgmQueuePlay(0, 1, 1, 0);
  self->base.phaseStep = 0;
  self->base.phase = self->base.phase + 1;
}
