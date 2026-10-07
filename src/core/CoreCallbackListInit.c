// bdc 0x08a2d1c0 CoreCallbackListInit
#include "bdc.h"

/* Constructor of the second compiled instance of the engine's priority-ordered linked list (the
   first is `CoreListInit`): clears `iterating` and `count`, allocates a node pool of `capacity`
   0x10-byte nodes from the low heap (`MemPoolInit`; no pool when `capacity < 1`) and takes (or
   allocates) a 0x10-byte sentinel node as head. Returns `list`. Used for the suspend/resume
   callback lists of `CorePower`, hence the name. */
CoreList *CoreCallbackListInit(CoreList *list, s32 capacity)
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
