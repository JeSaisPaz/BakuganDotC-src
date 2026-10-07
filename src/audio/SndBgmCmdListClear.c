// bdc 0x08a2fccc SndBgmCmdListClear
#include "bdc.h"

/* Frees every node behind the sentinel of the BGM command list (to the pool or the heap) and resets
   `count` to 0; the sentinel stays. The payload commands are not touched. Called by
   `SndBgmCmdListDestroy`. */
void SndBgmCmdListClear(CoreList *list)
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
