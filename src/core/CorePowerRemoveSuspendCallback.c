// bdc 0x089bcef8 CorePowerRemoveSuspendCallback
#include "bdc.h"

/* Removes `fn` from the `suspendCallbacks` list of `CorePower` (`CoreCallbackListRemove`) under
   the "COPower" lock and purges removed nodes. Returns 1 if the function was registered, else 0. */
int CorePowerRemoveSuspendCallback(CorePower *power, void *fn)
{
    int found;

    CoreLockAcquire(g_corePowerMgr->lock);
    found = CoreCallbackListRemove(power->suspendCallbacks, fn);
    CoreCallbackListPurgeRemoved(power->suspendCallbacks);
    CoreLockRelease(g_corePowerMgr->lock);
    return found;
}
