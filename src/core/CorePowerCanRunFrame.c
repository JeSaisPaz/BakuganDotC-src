// bdc 0x089bc944 CorePowerCanRunFrame
#include "bdc.h"

/* Tells the game thread whether it may run its normal start-of-frame work, read under the
   "COPower" lock. Returns 1 while a manual suspend is active or no resume is pending
   (`resumeNotify == 0`). With a resume pending it returns 0 while the state machine has not yet
   come back to state 0, and also in state 0 when `active` is set but the volatile region has not
   been locked again (`volatileLocked == 0`) — the window after a resume where `CorePowerStep`
   still has to re-acquire the volatile memory; otherwise 1. */
int CorePowerCanRunFrame(CorePower *power)
{
    int canRun;

    CoreLockAcquire(g_corePowerMgr->lock);
    if (power->manualSuspend != 0 || power->resumeNotify == 0) {
        canRun = 1;
    } else if (power->state != 0) {
        canRun = 0;
    } else if (power->active == 0 || power->volatileLocked != 0) {
        canRun = 1;
    } else {
        canRun = 0;
    }
    CoreLockRelease(g_corePowerMgr->lock);
    return canRun;
}
