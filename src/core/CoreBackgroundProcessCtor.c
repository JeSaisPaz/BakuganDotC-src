// bdc 0x089ff814 CoreBackgroundProcessCtor
#include "bdc.h"

/* Constructor of the `COBackGroundProcess` worker (`CoreBackgroundProcess`, 8 bytes): `jobs` is a
   0x10-byte callback list from the low heap (`CoreCallbackListInit` with an 8-node pool) and
   `lock` a 0x38-byte `CoreLock` LwMutex named "COBackGroundProcess"; either stays NULL when its
   allocation fails. Returns `proc`. */
CoreBackgroundProcess *CoreBackgroundProcessCtor(CoreBackgroundProcess *proc)
{
    bool fromLow;
    CoreList *jobs;
    CoreLock *lock;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    jobs = MemAlloc(sizeof(CoreList), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (jobs != NULL) {
        CoreCallbackListInit(jobs, 8);
    }
    proc->jobs = jobs;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    lock = MemAlloc(sizeof(CoreLock), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (lock != NULL) {
        CoreLockInit(lock, "COBackGroundProcess", CORE_LOCK_LWMUTEX);
    }
    proc->lock = lock;
    return proc;
}
