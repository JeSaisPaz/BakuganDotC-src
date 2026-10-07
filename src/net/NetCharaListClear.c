// bdc 0x08a31498 NetCharaListClear
#include "bdc.h"

/* Empties the net-character list: detaches the node chain from the sentinel, returns each node
   to the list's pool or, when there is no pool or the pool refuses it, frees it on the heap
   under the memory lock, then sets `count` to 0. The sentinel is kept. Called by
   `NetCharaListDestroy`. */
void NetCharaListClear(CoreList *list)
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
