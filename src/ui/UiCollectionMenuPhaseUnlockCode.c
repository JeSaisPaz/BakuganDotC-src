// bdc 0x089746d4 UiCollectionMenuPhaseUnlockCode
#include "bdc.h"

/* Phase 7 of `UiCollectionMenu`: fades out to black (16 frames), sets
   `g_uiKeepSharedBg` and opens the unlock-code screen (task 316, `UiUnlockCodeCtor`), waits for
   it to end, restores frame mode 0 and fades back in; once the fade-in is done (step 6) it returns
   to the main phase 2 at step 0x10. */

void UiCollectionMenuPhaseUnlockCode(UiCollectionMenu *self)
{
    GfxFader *fader;

    switch ((u32)self->base.phaseStep) {
    case 0:
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
        GfxFaderStart(GfxGetActiveFader(), 0x10);
        self->base.phaseStep++;
        break;
    case 1:
        if (GfxFaderIsFinished(GfxGetActiveFader())) {
            self->base.phaseStep++;
        }
        break;
    case 2:
        g_uiKeepSharedBg = 1;
        CoreTaskCreate(0x13c, 100);
        self->base.phaseStep++;
        break;
    case 3:
        if (CoreTaskExists(0x13c) == 0) {
            UiScreenSetFrameMode(&self->base.base, 0);
            self->base.phaseStep++;
        }
        break;
    case 4:
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
        self->base.phaseStep++;
        break;
    case 5:
        if (GfxFaderIsFinished(GfxGetActiveFader())) {
            self->base.phaseStep = 6;
        }
        break;
    default:
        self->base.phase = 2;
        self->base.phaseStep = 0x10;
        break;
    }
}
