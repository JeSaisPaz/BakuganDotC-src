// bdc 0x08808a30 UiLoadIconTaskUpdate
#include "bdc.h"

/* Update of the disc-access indicator task (`UiLoadIconTask`, id 2003). Without a disc manager
   (`IoDiscHasManager`) it just hides the icon (`UiLoadIconHide`). Otherwise the disc counts as
   busy when the reader is busy (`IoDiscIsBusy`); or, with a movie player present, when the UMD is
   not ready (`IoUmdIsMediaReady`); or, without one, when game thread 1 is not asleep
   (`BootIsThreadSleeping`), or when the camera task exists, profile flag 0 is set and the UMD is
   not ready. Script global bit 4 (`g_scriptGlobalBits`) forces "not busy". While busy it counts
   `busyFrames` (twice per update at frame-skip 1) and sets `visible` once past the power manager's
   threshold (`CorePowerGet``->unk24`); a visible icon is reset (counter and flag cleared) while
   the now-loading screen is open (`UiLoadingIsOpen`). When not busy a running counter and the flag
   are cleared. Finally a visible flag shows the icon (`UiLoadIconShow`); otherwise it is hidden
   and the threshold is reset to 60. */

void UiLoadIconTaskUpdate(CoreTask *task)
{
    UiLoadIconTask *self = (UiLoadIconTask *)task;
    bool busy;

    if (!IoDiscHasManager()) {
        UiLoadIconHide();
        return;
    }

    busy = false;
    if (IoDiscIsBusy(IoDiscGetManager())) {
        busy = true;
    } else if (GfxMovieHasPlayer()) {
        if (!IoUmdIsMediaReady()) {
            busy = true;
        }
    } else if (!BootIsThreadSleeping(1)) {
        busy = true;
    } else if (BtlCameraTaskExists() != 0 && SaveGetProfileFlag0() != 0 && !IoUmdIsMediaReady()) {
        busy = true;
    }

    if (CoreBitsetTest(4, g_scriptGlobalBits)) {
        busy = false;
    }

    if (busy) {
        self->busyFrames++;
        if (g_gfxDisplay->frameSkip > 0 && g_gfxDisplay->frameSkip < 2) {
            self->busyFrames++;
        }
        if (CorePowerGet()->unk24 < self->busyFrames) {
            self->visible = 1;
        }
        if (self->visible != 0 && UiLoadingIsOpen()) {
            self->busyFrames = 0;
            self->visible = 0;
        }
    } else if (self->busyFrames > 0) {
        self->busyFrames = 0;
        self->visible = 0;
    }

    if (self->visible != 0) {
        UiLoadIconShow();
    } else {
        UiLoadIconHide();
        CorePowerGet()->unk24 = 60;
    }
}
