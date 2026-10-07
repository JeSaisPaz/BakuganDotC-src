// bdc 0x08993cb4 UiUnlockCodeExitPhase
#include "bdc.h"

/* Phase 5 of `UiUnlockCode`. Step 0: initialises the fader slots when none is
   ready (sort key 20000.0), sets the active fader to fade from transparent black to opaque black
   over 8 frames (`GfxFaderStart`) and moves to step 1. Step 1: waits for
   `GfxFaderIsFinished`, then step 2. Both steps also run `UiUnlockCodeUpdateKeyPop`. Any
   other step sets `closeRequested` and `removeRequested`, resets step and phase to 0 and returns
   without the key pop. */
void UiUnlockCodeExitPhase(UiUnlockCode *self)
{
    GfxFader *fader;

    if (self->base.phaseStep == 0) {
        if (!GfxFaderIsReady()) {
            GfxFaderSlotsInit(0);
            fader = GfxGetActiveFader();
            fader->sortKey = 20000.0f;
        }
        fader = GfxGetActiveFader();
        fader->start[0] = 0.0f;
        fader->start[1] = 0.0f;
        fader->start[2] = 0.0f;
        fader->start[3] = 0.0f;
        fader = GfxGetActiveFader();
        fader->end[0] = 0.0f;
        fader->end[1] = 0.0f;
        fader->end[2] = 0.0f;
        fader->end[3] = 1.0f;
        GfxFaderStart(GfxGetActiveFader(), 8);
        self->base.phaseStep++;
    } else if (self->base.phaseStep == 1) {
        if (GfxFaderIsFinished(GfxGetActiveFader())) {
            self->base.phaseStep = 2;
        }
    } else {
        /* phaseStep < 0 or > 1 */
        self->base.closeRequested = 1;
        self->removeRequested = 1;
        self->base.phaseStep = 0;
        self->base.phase = 0;
        return;
    }
    UiUnlockCodeUpdateKeyPop(self);
}
