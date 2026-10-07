// bdc 0x08a2e5c0 SndEmitterListCreatePool
#include "bdc.h"

/* Creates the node pool of an emitter list: for `count > 0` it allocates a 0x14-byte header from
   the low heap and runs `MemPoolInit``(pool, 0x10, count, 1)` on it (16-byte nodes, slab from the
   low heap) and stores the pool at `list + 0x10`; for `count <= 0` it sets `list + 0x10` to NULL,
   so `SndEmitterListInsert` and the node allocation of `SndEmitterListInit` fall back to the
   heap. */
void SndEmitterListCreatePool(CorePrioList *list, s32 count)
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
