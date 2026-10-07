// bdc 0x08a30d8c CorePrioListCreatePool
#include "bdc.h"

/* Creates the node pool of a `CorePrioList`: for `count > 0` allocates a 0x14-byte header from
   the low heap and runs `MemPoolInit`(pool, 0x10, count, 1) on it (16-byte nodes, slab from the
   low heap), storing it in `pool` (NULL on allocation failure); otherwise sets `pool` to NULL. */
void CorePrioListCreatePool(CorePrioList *list, s32 count)
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
    pool = MemAlloc(sizeof(MemPool), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (pool != NULL)
        MemPoolInit(pool, sizeof(CorePrioNode), count, true);
    list->pool = pool;
}
