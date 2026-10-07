// bdc 0x08973c88 UiCollectionMenuPhaseStart
#include "bdc.h"

/* Phase 0 of `UiCollectionMenu`: waits a frame, starts the background effect
   `"main_bg.fab"` (`UiSharedAnimStart`), then starts a 16-frame fade-in from black
   (`GfxFaderStart`) and BGM 0x16 (`SndBgmQueuePlay`) and advances to phase 1. */

void UiCollectionMenuPhaseStart(UiCollectionMenu *self)
{
    GfxFader *fader;
    int step = self->base.phaseStep;

    if (step > 0) {
        if (step < 2) {
            UiSharedAnimStart(10.0f, 0.0f, 0.0f, self, (void *)"main_bg.fab", 0, 0);
            self->base.phaseStep = self->base.phaseStep + 1;
            return;
        }
    } else if (step >= 0) {
        self->base.phaseStep = step + 1;
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
    SndBgmQueuePlay(0, 0x16, 1, 0);
    self->base.phaseStep = 0;
    self->base.phase = self->base.phase + 1;
}
