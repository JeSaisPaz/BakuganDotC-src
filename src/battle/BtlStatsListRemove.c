// bdc 0x08a29d74 BtlStatsListRemove
#include "bdc.h"

/* Removes the first node of the unit-statistics list (`g_btlStatsList`, via `BtlStatsDestroy`)
   whose `data` equals `data` (byte-identical copy of `SndRequestListRemove`). While the list is
   not being iterated the node is unlinked at once, returned to the list's pool (`MemPoolFree`) or,
   when there is no pool or the pool rejects it, to the heap, and `count` is decremented. During
   iteration it is only flagged `removed` for the later purge. Returns true if a node was unlinked
   or newly flagged; false if no node matches, the list has no sentinel, or the first match is
   already flagged (the search stops there). */
bool BtlStatsListRemove(CoreList *list, void *data)
{
    CoreListNode *prev = list->sentinel;
    CoreListNode *node;

    if (prev == NULL) {
        return false;
    }
    for (node = prev->next; node != NULL; prev = node, node = node->next) {
        if (node->data != data) {
            continue;
        }
        if (list->iterating) {
            if (node->removed) {
                return false;
            }
            node->removed = 1;
            return true;
        }
        prev->next = node->next;
        node->next = NULL;
        if (node->removed) {
            return true;
        }
        if (list->pool == NULL || !MemPoolFree(list->pool, node)) {
            MemLock();
            MemFree(node, NULL, 0);
            MemUnlock();
        }
        list->count--;
        return true;
    }
    return false;
}
