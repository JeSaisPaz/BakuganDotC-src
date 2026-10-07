// bdc 0x08a2dbb8 CoreListClear
#include "bdc.h"

/* Empties a `CoreList`: detaches the node chain from the sentinel and frees every node (back
   to `pool` when it owns it, else `MemFree` under `MemLock`), then sets `count` to 0. The
   sentinel itself is kept. Called by `CoreListDestroy`. */
void CoreListClear(CoreList *list)
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
