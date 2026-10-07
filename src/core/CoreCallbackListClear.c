// bdc 0x08a326c4 CoreCallbackListClear
#include "bdc.h"

/* Empties a callback list: detaches the node chain after the sentinel, returns every node to the
   list's pool (`MemPoolFree`) or, when there is no pool or the pool refuses it, to the heap
   (`MemFree` under `MemLock`), and resets `count` to 0. The sentinel itself is kept. Called
   by `CoreCallbackListDestroy`. */
void CoreCallbackListClear(CoreList *list)
{
    CoreListNode *node;
    CoreListNode *next;

    if (list->sentinel != NULL) {
        node = list->sentinel->next;
        list->sentinel->next = NULL;
        while (node != NULL) {
            next = node->next;
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
