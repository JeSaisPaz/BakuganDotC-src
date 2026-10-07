// bdc 0x089bcfd4 CorePowerRemoveResumeCallback
#include "bdc.h"

/* Removes `fn` from the `resumeCallbacks` list of `CorePower` (`CoreCallbackListRemove`) under
   the "COPower" lock and purges removed nodes. Returns 1 if the function was registered, else 0.
   Counterpart of `CorePowerAddResumeCallback`. */
int CorePowerRemoveResumeCallback(CorePower *power, void *fn)
{
    int removed;

    CoreLockAcquire(g_corePowerMgr->lock);
    removed = CoreCallbackListRemove(power->resumeCallbacks, fn);
    CoreCallbackListPurgeRemoved(power->resumeCallbacks);
    CoreLockRelease(g_corePowerMgr->lock);
    return removed;
}
