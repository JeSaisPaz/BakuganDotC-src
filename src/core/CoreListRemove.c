// bdc 0x08a2d9d4 CoreListRemove
#include "bdc.h"

/* Removes the first node whose `data` is `data` from a `CoreList`. Outside a walk (`iterating
   == 0`) the node is unlinked, given back to the list's `MemPool` (or freed from the heap when
   it is not a pool node) and `count` is decremented. During a walk the node is only flagged
   `removed` for `CoreListPurgeRemoved`; an already flagged node counts as not found. Returns
   whether a node was removed or flagged. Called by `CoreTaskRemove`. */
bool CoreListRemove(CoreList *list, void *data)
{
    CoreListNode *prev = list->sentinel;
    CoreListNode *node = NULL;
    bool removed = false;

    if (prev != NULL) {
        for (node = prev->next; node != NULL; prev = node, node = node->next) {
            if (node->data != data) {
                continue;
            }
            if (!list->iterating) {
                removed = true;
                prev->next = node->next;
                node->next = NULL;
            } else if (!node->removed) {
                node->removed = 1;
                removed = true;
            }
            break;
        }
    }
    if (removed && !node->removed) {
        if (list->pool != NULL && MemPoolFree(list->pool, node)) {
            node = NULL;
        }
        if (node != NULL) {
            MemLock();
            MemFree(node, NULL, 0);
            MemUnlock();
        }
        list->count--;
    }
    return removed;
}
