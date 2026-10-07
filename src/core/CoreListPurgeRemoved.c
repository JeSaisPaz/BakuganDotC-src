// bdc 0x08a2dae8 CoreListPurgeRemoved
#include "bdc.h"

/* Unlinks and releases every node of a `CoreList` whose `removed` byte is set: the node goes back
   to the list's node pool (`MemPoolFree`) or, when there is no pool or the pool refuses it, is
   freed with `MemFree` under `MemLock`; `count` is decremented per node. Called at the end of
   `CoreTaskManagerDraw` so tasks removed during a frame disappear only after update and draw have
   both run. */
void CoreListPurgeRemoved(CoreList *list)
{
    CoreListNode *prev = list->sentinel;
    CoreListNode *node;

    if (prev == NULL) {
        return;
    }
    for (node = prev->next; node != NULL; node = prev->next) {
        if (!node->removed) {
            prev = node;
            continue;
        }
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
        list->count--;
    }
}
