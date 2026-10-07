// bdc 0x08a299d0 BtlStatsListClear
#include "bdc.h"

/* Byte-identical compiled copy of `NetAdhocListClear` for `BtlStatsList` (same instructions up to
   relocated addresses). */
void BtlStatsListClear(CoreList *list)
{
    CoreListNode *sentinel = list->sentinel;

    if (sentinel != NULL) {
        CoreListNode *node = sentinel->next;

        sentinel->next = NULL;
        while (node != NULL) {
            CoreListNode *next = node->next;

            if (list->pool == NULL || !MemPoolFree(list->pool, node)) {
                MemLock();
                MemFree(node, NULL, 0);
                MemUnlock();
            }
            node = next;
        }
    }
    list->count = 0;
}
