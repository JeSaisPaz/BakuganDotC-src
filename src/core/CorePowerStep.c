// bdc 0x089bd258 CorePowerStep
#include "bdc.h"

/* One step of the volatile-memory half of the power state machine, called by `CorePowerUpdate`
   with the "COPower" lock held. State 1 (suspend pending): when a volatile heap exists, the first
   call runs every callback of `suspendCallbacks` (list order, no arguments) and sets
   `suspendCbDone`; later calls wait until `Mem2GetUsedSize` of the volatile heap is 0, release
   the region with `CorePowerUnlockVolatileMem`, and once the region is no longer locked move to
   state 2 (clearing `suspendCbDone`). State 0 and below, and any state above 3: if the region is
   not locked, re-lock it with `CorePowerLockVolatileMem` and run every callback of
   `resumeCallbacks`. States 2 and 3 do nothing. */
void CorePowerStep(CorePower *power)
{
    s32 state;
    CoreList *callbacks;
    CoreListNode *node;

    state = power->state;
    if (state == 2 || state == 3)
        return;

    if (state == 1) {
        if (g_corePowerMgr->volatileHeap == NULL)
            return;
        if (!power->suspendCbDone) {
            callbacks = power->suspendCallbacks;
            if (callbacks != NULL && callbacks->count != 0) {
                for (node = CoreCallbackListFirst(callbacks); node != NULL; node = node->next)
                    ((void (*)(void))node->data)();
            }
            power->suspendCbDone = 1;
            return;
        }
        if (!power->volatileLocked) {
            power->state = 2;
            power->suspendCbDone = 0;
            return;
        }
        if (Mem2GetUsedSize(g_corePowerMgr->volatileHeap) != 0)
            return;
        CorePowerUnlockVolatileMem(power);
        return;
    }

    if (!power->volatileLocked) {
        CorePowerLockVolatileMem(power);
        callbacks = power->resumeCallbacks;
        if (callbacks != NULL && callbacks->count != 0) {
            for (node = CoreCallbackListFirst(callbacks); node != NULL; node = node->next)
                ((void (*)(void))node->data)();
        }
    }
}
