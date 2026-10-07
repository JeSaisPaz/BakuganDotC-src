// bdc 0x089bcf64 CorePowerAddResumeCallback
#include "bdc.h"

/* Registers `fn` in the `resumeCallbacks` list of `CorePower` (`CoreCallbackListInsert`,
   priority 1000, then `CoreCallbackListPurgeRemoved`) under the "COPower" lock and — unlike
   `CorePowerAddSuspendCallback` — also calls `fn()` once immediately, still holding the lock,
   so the new user can allocate its volatile memory right away. `CorePowerStep` calls every
   registered function again each time the volatile region has been re-acquired after a resume. */
void CorePowerAddResumeCallback(CorePower *power, void *fn)
{
    CoreLockAcquire(g_corePowerMgr->lock);
    CoreCallbackListInsert(power->resumeCallbacks, fn, 1000);
    CoreCallbackListPurgeRemoved(power->resumeCallbacks);
    ((void (*)(void))fn)();
    CoreLockRelease(g_corePowerMgr->lock);
}
