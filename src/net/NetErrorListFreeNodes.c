// bdc 0x08a31c14 NetErrorListFreeNodes
#include "bdc.h"

static void CorePrioChainFree(CorePrioList *list, CorePrioNode **chain)
{
    CorePrioNode *node;
    CorePrioNode *next;

    for (node = NetErrorNodeGetNext(*chain); node != NULL; node = next) {
        next = NetErrorNodeGetNext(node);
        if (list->pool != NULL && MemPoolFree(list->pool, node)) {
            NetErrorNodeDelete(node, 2);
            node = NULL;
        }
        if (node != NULL) {
            NetErrorNodeDelete(node, 3);
        }
    }
    NetErrorNodeSetNext(*chain, NULL);
}

/* Frees every node of both chains of the net-error list (pool nodes back to the pool, others
   deleted) and unlinks them. Byte-identical to `CorePrioListFreeNodes` and
   `SndEmitterListClear` (same template); called by `NetErrorListDestroy`. */
void NetErrorListFreeNodes(CorePrioList *list)
{
    CorePrioChainFree(list, &list->active);
    CorePrioChainFree(list, &list->pending);
}
