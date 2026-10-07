// bdc 0x08a309dc CorePrioListFreeNodes
#include "bdc.h"

/* Frees every node after the sentinel `*chain` (one of `list`'s two chains): a node that belongs to the list's `pool` is returned to it
   (`MemPoolFree`) and then destroyed with `CorePrioNodeDelete` flags 2; any other node is
   deleted with flags 3. Afterwards the sentinel's `next` link is NULL. */
static void CorePrioChainFree(CorePrioList *list, CorePrioNode **chain)
{
    CorePrioNode *node;
    CorePrioNode *next;

    for (node = CorePrioNodeGetNext(*chain); node != NULL; node = next) {
        next = CorePrioNodeGetNext(node);
        if (list->pool != NULL && MemPoolFree(list->pool, node)) {
            CorePrioNodeDelete(node, 2);
            node = NULL;
        }
        if (node != NULL) {
            CorePrioNodeDelete(node, 3);
        }
    }
    CorePrioNodeSetNext(*chain, NULL);
}

/* Frees every node after the two sentinels of both chains (`active` and `pending`): each node goes
   back to the pool (`MemPoolFree`; deleted with flags 2) when it belongs to it, otherwise it is
   deleted with `CorePrioNodeDelete` flags 3. Afterwards both sentinels' `next` links are NULL. */
void CorePrioListFreeNodes(CorePrioList *list)
{
    CorePrioChainFree(list, &list->active);
    CorePrioChainFree(list, &list->pending);
}
