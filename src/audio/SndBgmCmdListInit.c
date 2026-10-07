// bdc 0x08a2f844 SndBgmCmdListInit
#include "bdc.h"

/* Constructor of the BGM command list (`g_sndBgmCmdList`), a compiled copy of the `CoreList`
   class (same code as `CoreListInit`): clears `iterating` and `count`, builds a node pool of
   `capacity` 0x10-byte nodes from the low heap (no pool when `capacity < 1`) and takes one
   payload-less sentinel node from the pool, else from the heap, as the head. Returns `list`. */
CoreList *SndBgmCmdListInit(CoreList *list, s32 capacity)
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
