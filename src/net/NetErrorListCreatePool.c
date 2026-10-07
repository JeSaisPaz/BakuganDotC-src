// bdc 0x08a31fec NetErrorListCreatePool
#include "bdc.h"

/* Creates the node pool of a net-error list: for `count < 1` sets `pool` to NULL; otherwise
   allocates the 0x14-byte pool header from the low end of the heap (restoring the allocation
   direction) and, when that succeeds, initialises it for `count` 0x10-byte nodes. The pool
   pointer (possibly NULL) is stored in `list->pool`. Called by `NetErrorListInit`. */
void NetErrorListCreatePool(CorePrioList *list, s32 count)
{
    bool fromLow;
    MemPool *pool;

    if (count < 1) {
        list->pool = NULL;
        return;
    }
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    pool = MemAlloc(0x14, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (pool != NULL)
        MemPoolInit(pool, 0x10, count, true);
    list->pool = pool;
}
