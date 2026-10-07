// bdc 0x08854d04 BtlStatsDestroy
#include "bdc.h"

/* Releases a unit's statistics record (`BtlBakuganDtor`); does nothing unless both
   `g_btlStatsList` and `g_btlStatsPool` exist. When `MemPoolFree` accepts `stats` as a pool
   record, its deleting destructor (vtable entry 1) runs afterwards with flags 2 (no heap free);
   otherwise a non-NULL `stats` is destroyed with flags 3 (heap free). Then the record's node is
   removed from the list (`BtlStatsListRemove`) and, when that succeeds, flagged nodes are purged
   (`BtlStatsListPurgeRemoved`). */
void BtlStatsDestroy(BtlStats *stats)
{
    const VtblEntry *dtor;

    if (g_btlStatsList == NULL || g_btlStatsPool == NULL) {
        return;
    }
    if (MemPoolFree(g_btlStatsPool, stats)) {
        dtor = &stats->vtbl[1];
        ((void (*)(void *, s32))dtor->fn)((u8 *)stats + dtor->delta, 2);
    } else if (stats != NULL) {
        dtor = &stats->vtbl[1];
        ((void (*)(void *, s32))dtor->fn)((u8 *)stats + dtor->delta, 3);
    }
    if (BtlStatsListRemove(g_btlStatsList, stats)) {
        BtlStatsListPurgeRemoved(g_btlStatsList);
    }
}
