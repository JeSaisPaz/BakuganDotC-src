// bdc 0x089bc80c CorePowerStateInit
#include "bdc.h"

/* Constructor of `CorePower`: puts the state machine in the running state (`state = 0`, `active =
   1`, volatile region not locked, manual/resume flags cleared, `unk24 = 0x78`) and builds the two
   callback lists `suspendCallbacks` and `resumeCallbacks` with `CoreCallbackListInit``(list, 4)`,
   each in a 0x10-byte block taken from the low end of the heap (NULL when the allocation fails).
   Returns `power`. */
static void *AllocCallbackList(void)
{
    bool fromLow;
    void *list;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    list = MemAlloc(0x10, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (list != NULL) {
        CoreCallbackListInit(list, 4);
    }
    return list;
}

CorePower *CorePowerStateInit(CorePower *power)
{
    power->lastFlags = 0;
    power->volatileLocked = 0;
    power->volatileBase = NULL;
    power->volatileSize = 0;
    power->state = 0;
    power->active = 1;
    power->unk16 = 0;
    power->resumeNotify = 0;
    power->unk18 = 0;
    power->suspendCbDone = 0;
    power->unk1c = 0;
    power->unk20 = 0;
    power->manualSuspend = 0;
    power->manualResume = 0;
    power->suspendCallbacks = AllocCallbackList();
    power->resumeCallbacks = AllocCallbackList();
    power->unk24 = 0x78;
    return power;
}
