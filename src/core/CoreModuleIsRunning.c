// bdc 0x089cda94 CoreModuleIsRunning
#include "bdc.h"

/* Returns 1 when module slot `slot` (of the `g_coreModuleSlots` table) is running (state 3), read
   under the manager lock; 0 otherwise. Polled after `CoreModuleRequestLoad`. */
int CoreModuleIsRunning(CoreModuleMgr *mgr, int slot)
{
    int running = 0;

    CoreLockAcquire(mgr->lock);
    if (slot >= 0 && slot < 8 && g_coreModuleSlots[slot].state == 3) {
        running = 1;
    }
    CoreLockRelease(mgr->lock);
    return running;
}
