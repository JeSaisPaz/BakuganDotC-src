// bdc 0x0885499c BtlStatsSystemInit
#include "bdc.h"

/* Creates the battle statistics system when `g_btlStatsList` does not exist yet: allocates the
   `CoreList` header from the low heap and builds it with `BtlStatsListInit` (`capacity` nodes),
   then allocates the `MemPool` header the same way and builds a pool of `capacity` `BtlStats`
   records (0x170 bytes each, from the low heap) with `MemPoolInit`. A failed allocation leaves
   the global NULL. When the list already exists it returns without touching either global.
   Called by `BtlMainTaskCtor` and `BtlMainTeardown`. */
void BtlStatsSystemInit(s32 capacity)
{
    bool fromLow;
    CoreList *list;
    MemPool *pool;

    if (g_btlStatsList != NULL) {
        return;
    }

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    list = MemAlloc(sizeof(CoreList), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (list != NULL) {
        BtlStatsListInit(list, capacity);
    }
    g_btlStatsList = list;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    pool = MemAlloc(sizeof(MemPool), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (pool != NULL) {
        MemPoolInit(pool, sizeof(BtlStats), capacity, true);
    }
    g_btlStatsPool = pool;
}
