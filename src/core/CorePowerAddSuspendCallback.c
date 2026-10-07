// bdc 0x089bce90 CorePowerAddSuspendCallback
#include "bdc.h"

/* Registers `fn` in the `suspendCallbacks` list of `CorePower` (`CoreCallbackListInsert` with
   priority 1000, lower priorities run first) under the "COPower" lock, then lets
   `CoreCallbackListPurgeRemoved` clean up. `CorePowerStep` calls every registered function
   once, with no arguments, when a suspend starts, giving users of volatile memory the chance to
   free it. */
void CorePowerAddSuspendCallback(CorePower *power, void *fn)
{
    CoreLockAcquire(g_corePowerMgr->lock);
    CoreCallbackListInsert(power->suspendCallbacks, fn, 1000);
    CoreCallbackListPurgeRemoved(power->suspendCallbacks);
    CoreLockRelease(g_corePowerMgr->lock);
}
