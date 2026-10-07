// bdc 0x08a2f618 SndGroupIdListPurgeRemoved
#include "bdc.h"

/* Walks the needed-group-id list of the `SndGroupLoader` (`neededGroups`, data = the group id
   itself, stored directly in the node) and unlinks every node flagged `removed`, freeing each one
   to the list's pool (or the heap) and decrementing `count`. Run after iteration finishes to
   complete the deferred removals of the remove function. */

void SndGroupIdListPurgeRemoved(CoreList *list)
{
    CoreListNode *prev;
    CoreListNode *node;

    prev = list->sentinel;
    if (prev == NULL) {
        return;
    }
    node = prev->next;
    while (node != NULL) {
        if (node->removed == 0) {
            prev = node;
            node = prev->next;
        } else {
            prev->next = node->next;
            node->next = NULL;
            if (list->pool != NULL && MemPoolFree(list->pool, node)) {
                node = NULL;
            }
            if (node != NULL) {
                MemLock();
                MemFree(node, NULL, 0);
                MemUnlock();
            }
            list->count = list->count - 1;
            node = prev->next;
        }
    }
}
