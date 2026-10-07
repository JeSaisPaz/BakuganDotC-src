// bdc 0x0889d9a4 BtlStageResumeEventScript
#include "bdc.h"

/* Clears the `paused` byte of the battle event script `g_btlEventScript` when it exists.
   Counterpart of `BtlStageSuspendEventScript`; called e.g. by `BtlCameraEnterDefaultMode`. */
void BtlStageResumeEventScript(void)
{
    if (g_btlEventScript != NULL) {
        g_btlEventScript->paused = 0;
    }
}
