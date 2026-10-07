// bdc 0x08a2d660 CoreListInit
#include "bdc.h"

/* Constructor of the engine's priority-ordered linked list (`CoreList`): for `capacity > 0`
   allocates a 0x14-byte `MemPool` header from the low heap and initialises it with `capacity`
   0x10-byte nodes (`MemPoolInit`; no pool when `capacity < 1` or the allocation fails), clears
   `iterating`/`count`, then takes a sentinel node from the pool or, failing that, from the low heap
   (its `next` and `removed` cleared; NULL when both fail). Returns the list.
   `CoreTaskManagerInit` builds `g_taskList` with `capacity` 0x20. */
CoreList *CoreListInit(CoreList *list, s32 capacity)
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
        if (pool != NULL)
            MemPoolInit(pool, sizeof(CoreListNode), capacity, true);
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
