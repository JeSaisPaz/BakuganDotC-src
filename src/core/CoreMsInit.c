// bdc 0x089faea8 CoreMsInit
#include "bdc.h"

/* Creates the memory-stick service once (no-op when `g_coreMsMgr` is already set): from the low
   end of the heap it allocates and zero-fills a 0x80-byte `CoreMsMgr`, a 300-byte `CoreMs`
   (built by `CoreMsStateInit`, so the "MyCB-MS" callback is registered) and a 0x38-byte
   `CoreLock` named "COMS::Create" (type `CORE_LOCK_LWMUTEX`), and sets `devSizePtr = &devSize`.
   Allocation failures leave the matching field NULL (the manager itself is not checked). Called
   by `main` after `CoreStopwatchInit`. */
void CoreMsInit(void)
{
    bool fromLow;
    CoreMs *ms;
    CoreLock *lock;

    if (g_coreMsMgr != NULL)
        return;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    g_coreMsMgr = MemAlloc(sizeof(CoreMsMgr), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    memset(g_coreMsMgr, 0, sizeof(CoreMsMgr));

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    ms = MemAlloc(sizeof(CoreMs), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (ms != NULL)
        CoreMsStateInit(ms);
    g_coreMsMgr->ms = ms;
    g_coreMsMgr->devSizePtr = &g_coreMsMgr->devSize;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    lock = MemAlloc(sizeof(CoreLock), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (lock != NULL)
        CoreLockInit(lock, "COMS::Create", CORE_LOCK_LWMUTEX);
    g_coreMsMgr->lock = lock;
}
