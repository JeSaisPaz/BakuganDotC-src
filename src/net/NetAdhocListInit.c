// bdc 0x08a2e774 NetAdhocListInit
#include "bdc.h"

/* Constructor of a `CoreList`-shaped list (the `CoreListInit` compiler copy whose instance is
   owned by the ad-hoc network manager): clears `iterating`/`count`, builds a node pool of
   `capacity` 0x10-byte nodes from the low heap (no pool when `capacity < 1`, then `pool = NULL`)
   and takes — from the pool, else from the low heap — one 0x10-byte payload-less sentinel node
   (`next = NULL`, `removed = 0`) as the head. Returns `list`. The ad-hoc manager builds its list
   (`+0x1f8` of `g_netAdhoc`) with capacity 0x20. */
CoreList *NetAdhocListInit(CoreList *list, s32 capacity)
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
        pool = MemAlloc(sizeof(MemPool), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (pool != NULL) {
            MemPoolInit(pool, sizeof(CoreListNode), capacity, true);
        }
        list->pool = pool;
    }
    list->iterating = 0;
    list->count = 0;
    list->sentinel = NULL;

    sentinel = NULL;
    if (pool != NULL) {
        sentinel = MemPoolAlloc(pool);
    }
    if (sentinel == NULL) {
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        sentinel = MemAlloc(sizeof(CoreListNode), NULL, 0);
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
