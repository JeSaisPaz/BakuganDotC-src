// bdc 0x08a2f374 SndGroupIdListInit
#include "bdc.h"

/* Constructor of the needed-group-id list of the `SndGroupLoader` (`neededGroups`, the set of
   sound-group ids that the live requests need, rebuilt every `SndGroupLoaderUpdate`): a compiled
   copy of `CoreListInit`. Creates a node pool of `capacity` 0x10-byte nodes on the low heap
   (`MemPoolInit(pool, 0x10, capacity, 1)`, stored at `list + 0xc`; no pool when `capacity < 1`),
   clears `iterating` and `count`, and takes a payload-less sentinel node (`next = NULL`, `removed =
   0`) from the pool, or from the low heap when the pool is missing or full, as `list + 8`. Returns
   `list`. */
CoreList *SndGroupIdListInit(CoreList *list, s32 capacity)
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
