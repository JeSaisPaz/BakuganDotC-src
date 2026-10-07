// bdc 0x08a313c8 NetCharaListPurgeRemoved
#include "bdc.h"

/* Unlinks and frees every node flagged `removed` in the net-character list (back to the pool,
   else on the heap under the memory lock), decrementing `count` for each. Does nothing without a
   sentinel. Called by `NetCharaMgrUpdate`, `NetCharaMgrRemoveRemotes` and
   `NetCharaMgrDestroy` after walks that flag characters for removal. */
void NetCharaListPurgeRemoved(void *list)
{
    CoreList *l = list;
    CoreListNode *prev;
    CoreListNode *node;

    prev = l->sentinel;
    if (prev == NULL)
        return;
    node = prev->next;
    while (node != NULL) {
        if (!node->removed) {
            prev = node;
            node = node->next;
            continue;
        }
        prev->next = node->next;
        node->next = NULL;
        if (l->pool == NULL || !MemPoolFree(l->pool, node)) {
            MemLock();
            MemFree(node, NULL, 0);
            MemUnlock();
        }
        l->count--;
        node = prev->next;
    }
}
