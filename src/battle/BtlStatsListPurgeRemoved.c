// bdc 0x08a29ca4 BtlStatsListPurgeRemoved
#include "bdc.h"

/* Unlinks and releases every node of a `CoreCallbackListInit` list (`CoreList`) whose
   `removed` byte is set: the node goes back to the list's node pool (`MemPoolFree`) or, when
   there is no pool or the pool refuses it, is freed with `MemFree` under `MemLock`; `count`
   is decremented per node. Second compiled instance of `CoreListPurgeRemoved`. */
void BtlStatsListPurgeRemoved(void *list)
{
    CoreList *l = list;
    CoreListNode *prev;
    CoreListNode *node;

    prev = l->sentinel;
    if (prev == NULL)
        return;
    node = prev->next;
    while (node != NULL) {
        if (!node->removed) {
            prev = node;
            node = node->next;
            continue;
        }
        prev->next = node->next;
        node->next = NULL;
        if (l->pool == NULL || !MemPoolFree(l->pool, node)) {
            MemLock();
            MemFree(node, NULL, 0);
            MemUnlock();
        }
        l->count--;
        node = prev->next;
    }
}
