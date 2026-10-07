// bdc 0x089c124c SndGroupLoaderInit
#include "bdc.h"

/* Constructor of the `SndGroupLoader` (the original class is probably `COSeFileMng`, after its
   lock name). Every sub-object is allocated from the low heap under `MemLock`: the three sorted
   lists `requests` (`SndRequestListInit`), `holds` (`SndGroupHoldListInit`) and `neededGroups`
   (`SndGroupIdListInit`), each a 0x10-byte header with capacity 0x20; the 0x38-byte `CoreLock`
   `lock` named `"COSeFileMng"` (`CoreLockInit(lock, "COSeFileMng", CORE_LOCK_LWMUTEX)`); the
   request pool `MemPoolInit(pool, 0xc, 0x20, 1)` (12-byte request records) and the hold pool
   `MemPoolInit(pool, 8, 0x20, 1)` (8-byte hold records). Finally `enabled` (+0x18) is cleared.
   Returns `self`; a failed allocation leaves the field NULL. */
SndGroupLoader *SndGroupLoaderInit(SndGroupLoader *self)
{
    bool fromLow;
    CoreList *list;
    CoreLock *lock;
    MemPool *pool;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    list = MemAlloc(0x10, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (list != NULL)
        SndRequestListInit(list, 0x20);
    self->requests = list;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    list = MemAlloc(0x10, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (list != NULL)
        SndGroupHoldListInit(list, 0x20);
    self->holds = list;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    list = MemAlloc(0x10, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (list != NULL)
        SndGroupIdListInit(list, 0x20);
    self->neededGroups = list;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    lock = MemAlloc(0x38, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (lock != NULL)
        CoreLockInit(lock, "COSeFileMng", CORE_LOCK_LWMUTEX);
    self->lock = lock;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    pool = MemAlloc(0x14, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (pool != NULL)
        MemPoolInit(pool, 0xc, 0x20, true);
    self->requestPool = pool;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    pool = MemAlloc(0x14, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (pool != NULL)
        MemPoolInit(pool, 8, 0x20, true);
    self->holdPool = pool;
    self->enabled = 0;
    return self;
}
