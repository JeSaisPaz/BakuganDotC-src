// bdc 0x08a30f40 NetCharaListInit
#include "bdc.h"

/* Constructor of a `NetCharaList`, a byte-identical compiled copy of `CoreListInit`: for
   `capacity > 0` allocates a 0x14-byte `MemPool` from the low heap and initialises it with
   `capacity` 0x10-byte nodes (`MemPoolInit`; no pool when `capacity < 1` or the allocation
   fails), clears `iterating`/`count`, then takes a sentinel node from the pool or, failing that,
   from the low heap (its `next` and `removed` cleared; NULL when both fail). Returns the list. */
NetCharaList *NetCharaListInit(NetCharaList *list, s32 capacity)
{
    bool fromLow;
    MemPool *pool;
    NetCharaListNode *sentinel;

    if (capacity < 1) {
        list->pool = NULL;
        pool = NULL;
    } else {
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        pool = MemAlloc(0x14, NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (pool != NULL)
            MemPoolInit(pool, 0x10, capacity, true);
        list->pool = pool;
    }
    list->iterating = 0;
    list->count = 0;
    list->sentinel = NULL;

    sentinel = NULL;
    if (pool != NULL)
        sentinel = MemPoolAlloc(pool);
    if (sentinel == NULL) {
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        sentinel = MemAlloc(0x10, NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
    }
    if (sentinel != NULL) {
        sentinel->next = NULL;
        sentinel->removed = 0;
    }
    list->sentinel = sentinel;
    return list;
}
