// bdc 0x089cdb10 CoreModuleIsUnloaded
#include "bdc.h"

/* Returns 1 when module slot `slot` is unloaded (state 0), read under the manager lock; 0
   otherwise. Polled after `CoreModuleRequestUnload`. */
int CoreModuleIsUnloaded(CoreModuleMgr *mgr, int slot)
{
    int unloaded = 0;

    CoreLockAcquire(mgr->lock);
    if (slot >= 0 && slot < 8 && g_coreModuleSlots[slot].state == 0) {
        unloaded = 1;
    }
    CoreLockRelease(mgr->lock);
    return unloaded;
}
