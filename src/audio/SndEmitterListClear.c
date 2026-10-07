// bdc 0x08a2e210 SndEmitterListClear
#include "bdc.h"

static void CorePrioChainFree(CorePrioList *list, CorePrioNode **chain)
{
    CorePrioNode *node;
    CorePrioNode *next;

    for (node = SndEmitterNodeGetNext(*chain); node != NULL; node = next) {
        next = SndEmitterNodeGetNext(node);
        if (list->pool != NULL && MemPoolFree(list->pool, node)) {
            SndEmitterNodeFree(node, 2);
            node = NULL;
        }
        if (node != NULL) {
            SndEmitterNodeFree(node, 3);
        }
    }
    SndEmitterNodeSetNext(*chain, NULL);
}

/* Empties both chains: frees every node after each sentinel (to the pool via `MemPoolFree` or to
   the heap via `SndEmitterNodeFree`) and sets both sentinels' `next` to NULL. */
void SndEmitterListClear(CorePrioList *list)
{
    CorePrioChainFree(list, &list->active);
    CorePrioChainFree(list, &list->pending);
}
