// bdc 0x089cd9d4 CoreModuleRequestUnload
#include "bdc.h"

/* Requests unloading of module slot `slot` (0..7): under the manager lock, a running slot
   (state 3) is set to 4 and the DevModule thread is woken. Returns 1 when posted or the slot is
   already unloaded (state 0), else 0 (bad slot, other states, failed wakeup). */
int CoreModuleRequestUnload(void *mgr, int slot)
{
    CoreModuleMgr *self = mgr;
    int ok = 0;

    CoreLockAcquire(self->lock);
    if (slot >= 0 && slot < 8) {
        CoreModuleSlot *entry = &g_coreModuleSlots[slot];
        s32 state = entry->state;

        if (state <= 0) {
            if (state == 0) {
                ok = 1;
            }
        } else if (state == 3) {
            SceUID thid;

            entry->state = 4;
            thid = BootGetThreadId(1);
            if (thid > 0 && sceKernelWakeupThread(thid) == 0) {
                ok = 1;
            }
        }
    }
    CoreLockRelease(self->lock);
    return ok;
}
