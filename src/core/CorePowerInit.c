// bdc 0x089bc678 CorePowerInit
#include "bdc.h"

/* Creates the power service once (no-op when `g_corePowerMgr` is already set): allocates the
   12-byte `CorePowerMgr` (zeroed) and a 0x30-byte `CorePower` (initialised by
   `CorePowerStateInit`) and a 0x38-byte `CoreLock` named "COPower" (type `CORE_LOCK_MUTEX`,
   `CoreLockInit`), all from the low end of the heap, and links them into the manager (NULL for
   a failed allocation). Called by `main` right after the heap exists. */

static inline void *AllocLow(u32 size)
{
    bool fromLow;
    void *p;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    p = MemAlloc(size, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    return p;
}

void CorePowerInit(void)
{
    CorePower *power;
    CoreLock *lock;

    if (g_corePowerMgr != NULL) {
        return;
    }
    g_corePowerMgr = AllocLow(0xc);
    memset(g_corePowerMgr, 0, 0xc);

    power = AllocLow(0x30);
    if (power != NULL) {
        CorePowerStateInit(power);
    }
    g_corePowerMgr->power = power;

    lock = AllocLow(0x38);
    if (lock != NULL) {
        CoreLockInit(lock, "COPower", CORE_LOCK_MUTEX);
    }
    g_corePowerMgr->lock = lock;
}
