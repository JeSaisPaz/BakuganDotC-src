// bdc 0x08a2e98c NetAdhocListClear
#include "bdc.h"

/* Frees every node behind the sentinel of the ad-hoc manager's `CoreList` copy: detaches the
   chain (`sentinel->next = NULL`), then walks it and returns each node to the pool (`MemPoolFree`
   non-zero) or frees it on the heap (`MemFree` under `MemLock`), and finally resets `count` to 0.
   The sentinel itself is kept. Called by `NetAdhocListDestroy`. */
void NetAdhocListClear(CoreList *list)
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
