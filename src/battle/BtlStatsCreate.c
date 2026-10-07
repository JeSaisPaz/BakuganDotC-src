// bdc 0x08854c20 BtlStatsCreate
#include "bdc.h"

/* Allocates a statistics record for a new battle unit (`BtlBakuganCtor`). Returns NULL unless
   both `g_btlStatsList` and `g_btlStatsPool` exist. Takes the record from the pool
   (`MemPoolAlloc`) or, when the pool is full, from the low heap (`MemAlloc` of 0x170 bytes
   under `MemLock`), constructs it (`BtlStatsCtor`, skipped when the heap allocation failed),
   inserts it into the list with priority 1000 (`BtlStatsListInsert`, even when NULL) and purges
   the list's removed nodes (`BtlStatsListPurgeRemoved`). Returns the record. */
BtlStats *BtlStatsCreate(void)
{
    BtlStats *stats = NULL;

    if (g_btlStatsList == NULL || g_btlStatsPool == NULL) {
        return stats;
    }
    stats = MemPoolAlloc(g_btlStatsPool);
    if (stats != NULL) {
        BtlStatsCtor(stats);
    } else {
        bool fromLow;
        BtlStats *heap;

        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        heap = MemAlloc(sizeof(BtlStats), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (heap != NULL) {
            BtlStatsCtor(heap);
            stats = heap;
        }
    }
    BtlStatsListInsert(g_btlStatsList, stats, 1000);
    BtlStatsListPurgeRemoved(g_btlStatsList);
    return stats;
}
