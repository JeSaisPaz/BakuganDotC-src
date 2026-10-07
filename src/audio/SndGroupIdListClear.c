// bdc 0x08a2f6e8 SndGroupIdListClear
#include "bdc.h"

/* Empties the needed-group-id list of the `SndGroupLoader` (`neededGroups`): frees every node
   after the sentinel (to the list's `MemPool` or the heap) without looking at `removed`, sets
   `sentinel->next = NULL` and `count = 0`. `SndGroupLoaderUpdate` clears it at the start of every
   frame and rebuilds it from the live requests. */
void SndGroupIdListClear(CoreList *list)
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
