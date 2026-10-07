// bdc 0x08854aac BtlStatsSystemShutdown
#include "bdc.h"

/* Shuts the battle statistics system down when `g_btlStatsList` exists: walks the list from its
   first node (`BtlStatsListFirst`), and for every node with a `BtlStats` payload calls the
   record's destructor (vtable entry 1, flags 2 = destruct only) and hands it back to
   `g_btlStatsPool` with `MemPoolFree` (its result is ignored, so a record that came from the
   heap fallback is not freed); then destroys the list (`BtlStatsListDestroy`, flags 3) and the pool
   (`MemPoolDestroy`, flags 3) and clears both globals. */
void BtlStatsSystemShutdown(void)
{
    CoreList *list;
    CoreListNode *node;

    if (g_btlStatsList == NULL) {
        return;
    }
    list = g_btlStatsList;
    node = BtlStatsListFirst(g_btlStatsList);
    while (node != NULL) {
        BtlStats *stats = node->data;

        node = node->next;
        if (stats != NULL) {
            const VtblEntry *dtor = &stats->vtbl[1];

            ((void (*)(void *, s32))dtor->fn)((u8 *)stats + dtor->delta, 2);
            MemPoolFree(g_btlStatsPool, stats);
        }
        list = g_btlStatsList;
    }
    if (list != NULL) {
        BtlStatsListDestroy(list, 3);
        g_btlStatsList = NULL;
    }
    if (g_btlStatsPool != NULL) {
        MemPoolDestroy(g_btlStatsPool, 3);
        g_btlStatsPool = NULL;
    }
}
