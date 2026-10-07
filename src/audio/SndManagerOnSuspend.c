// bdc 0x089c6b2c SndManagerOnSuspend
#include "bdc.h"

/* Power-suspend callback of the sound manager (registered by `SndManagerStateInitAudio` with
   `CorePowerAddSuspendCallback`): sets `suspended = 1` and `buffersReady = 0` (`SndManager +0x1c`
   / `+0x1d`), asks BGM player 0 to suspend (`SndBgmPlayerRequestSuspend`), stops BGM player 1
   (`SndBgmPlayerStop`, 0 ms) and asks it to suspend too, and clears the pointer to the
   volatile-memory block (`+0x18`) because that memory does not survive the suspend. Counterpart:
   `SndManagerOnResume`. */

void SndManagerOnSuspend(void)
{
    SndManager *mgr = SndGetManager();

    mgr->suspended = 1;
    mgr->buffersReady = 0;
    if (SndBgmPlayerExists(0)) {
        SndBgmPlayerRequestSuspend(SndBgmPlayerGet(0));
    }
    if (SndBgmPlayerExists(1)) {
        SndBgmPlayerStop(SndBgmPlayerGet(1), 0, 0);
        SndBgmPlayerRequestSuspend(SndBgmPlayerGet(1));
    }
    mgr->volatileBlock = NULL;
}
