// bdc 0x08a29870 BtlStatsListInit
#include "bdc.h"

/* Constructor of the battle statistics list `g_btlStatsList` (byte-identical copy of
   `CoreListInit`; called by `BtlStatsSystemInit`): for `capacity > 0` allocates a 0x14-byte
   `MemPool` header from the low heap and initialises it with `capacity` 0x10-byte nodes (no pool
   when `capacity < 1` or the allocation fails), clears `iterating`/`count`, then takes a sentinel
   node from the pool or, failing that, from the low heap (its `next` and `removed` cleared; NULL
   when both fail). Returns the list. */
CoreList *BtlStatsListInit(CoreList *list, s32 capacity)
{
    bool fromLow;
    MemPool *pool;
    CoreListNode *sentinel;

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
