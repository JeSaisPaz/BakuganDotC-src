// bdc 0x089cd920 CoreModuleRequestLoad
#include "bdc.h"

/* Requests loading of module slot `slot` (0..7): under the manager lock, if the slot is unloaded
   (state 0) sets it to 1, wakes the DevModule thread (slot 1 of `g_threadTable`,
   `BootGetThreadId`) and bumps `liveCount`. Returns 1 when the request was posted or the slot
   was already busy/loaded, 0 on a bad slot or failed wakeup (the state stays 1 then). */
int CoreModuleRequestLoad(void *mgr, int slot)
{
    CoreModuleMgr *self = mgr;
    int ok = 0;

    CoreLockAcquire(self->lock);
    if (slot >= 0 && slot < 8) {
        CoreModuleSlot *entry = &g_coreModuleSlots[slot];

        if (entry->state == 0) {
            SceUID thid;

            entry->state = 1;
            thid = BootGetThreadId(1);
            if (thid > 0 && sceKernelWakeupThread(thid) == 0) {
                ok = 1;
                self->liveCount++;
            }
        } else {
            ok = 1;
        }
    }
    CoreLockRelease(self->lock);
    return ok;
}
