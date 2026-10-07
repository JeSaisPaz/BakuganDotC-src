// bdc 0x0891ce1c UiHologramGalleryLoadPhase
#include "bdc.h"

/* Phase 0 of the hologram gallery, a sub-state machine on `phaseStep`:
   0: start loading voice pac 0x6c (if the loader is busy and not idle, release the old pac);
   1: wait until the voice pac is loaded;
   2: build the screen (`UiHologramGalleryInitSprites`);
   3: start a 16-frame fade from opaque black to transparent and, when
      `UiHologramGalleryEnsureVoicePac` returns 1, queue BGM 1 on channel 0 (looping);
   other (>= 4 or negative): once task 0x2756 is gone and stream file 1 is loaded, reset
   `phaseStep` and advance `phase`. */

void UiHologramGalleryLoadPhase(UiHologramGallery *self)
{
  GfxFader *fader;
  int step = self->base.phaseStep;

  switch (step) {
  case 0:
    if (SndVoicePacLoad(0x6c)) {
      self->base.phaseStep++;
      return;
    }
    if (SndVoicePacIsIdle())
      return;
    SndVoicePacRelease();
    return;
  case 1:
    if (!SndVoicePacIsLoaded())
      return;
    self->base.phaseStep++;
    return;
  case 2:
    UiHologramGalleryInitSprites(self);
    self->base.phaseStep++;
    return;
  case 3:
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
    GfxFaderStart(GfxGetActiveFader(), 0x10);
    if (UiHologramGalleryEnsureVoicePac() == 1)
      SndBgmQueuePlay(0, 1, 1, 0);
    self->base.phaseStep++;
    return;
  default:
    if (CoreTaskExists(0x2756) == 0 && SndStreamFileIsLoaded(1)) {
      self->base.phaseStep = 0;
      self->base.phase++;
    }
    return;
  }
}
