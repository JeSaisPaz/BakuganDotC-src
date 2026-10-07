// bdc 0x08a29a78 BtlStatsListDestroy
#include "bdc.h"

/* Destructor of the battle stats list `BtlStatsList` (a `CoreList`, called by
   `BtlStatsSystemShutdown`): empties it (`BtlStatsListClear`), returns the sentinel to the
   node pool when there is one, destroys the pool, frees the sentinel with `MemFree` if it is
   still held, and frees the list object itself when bit 0 of `flags` is set. A NULL `list` does
   nothing. */
void BtlStatsListDestroy(CoreList *list, u32 flags)
{
    if (list == NULL) {
        return;
    }
    BtlStatsListClear(list);
    if (list->pool != NULL) {
        if (MemPoolFree(list->pool, list->sentinel)) {
            list->sentinel = NULL;
        }
        if (list->pool != NULL) {
            MemPoolDestroy(list->pool, 3);
            list->pool = NULL;
        }
    }
    if (list->sentinel != NULL) {
        CoreListNode *sentinel = list->sentinel;

        MemLock();
        MemFree(sentinel, NULL, 0);
        MemUnlock();
        list->sentinel = NULL;
    }
    if (flags & 1) {
        MemLock();
        MemFree(list, NULL, 0);
        MemUnlock();
    }
}
