// bdc 0x089ad4dc UiPauseSettingsButtonFlashDone
#include "bdc.h"

/* Steps flash slot 0 (`UiFlashStep`); returns 1 when finished, 0 while still running. */

int UiPauseSettingsButtonFlashDone(UiPauseSettings *self)
{
    if (UiFlashStep(0) != 0) {
        return 1;
    }
    return 0;
}
