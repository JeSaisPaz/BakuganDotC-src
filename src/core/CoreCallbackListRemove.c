// bdc 0x08a2d464 CoreCallbackListRemove
#include "bdc.h"

/* Removes the first node of a `CoreCallbackListInit` list (`CoreList` layout) whose payload
   equals `payload`. Returns 1 when a node was removed, else 0. If the list is not being iterated
   (`iterating == 0`) the node is unlinked, returned to the pool (or freed on the heap under
   `MemLock`) and `count` is decremented; otherwise it is only flagged `removed` so that the
   iterating loop can finish and `CoreCallbackListPurgeRemoved` reclaims it later (a node already
   flagged returns 0). */
int CoreCallbackListRemove(void *list, void *payload)
{
    CoreList *l = list;
    CoreListNode *prev;
    CoreListNode *node;
    int removed;

    removed = 0;
    node = NULL;
    prev = l->sentinel;
    if (prev != NULL) {
        for (node = prev->next; node != NULL; node = node->next) {
            if (node->data == payload) {
                if (l->iterating == 0) {
                    removed = 1;
                    prev->next = node->next;
                    node->next = NULL;
                } else if (node->removed == 0) {
                    node->removed = 1;
                    removed = 1;
                }
                break;
            }
            prev = node;
        }
    }
    if (removed && node->removed == 0) {
        if (l->pool != NULL && MemPoolFree(l->pool, node)) {
            node = NULL;
        }
        if (node != NULL) {
            MemLock();
            MemFree(node, NULL, 0);
            MemUnlock();
        }
        l->count = l->count - 1;
    }
    return removed;
}
