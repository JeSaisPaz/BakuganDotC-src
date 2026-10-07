// bdc 0x08a30038 SndEmitterGroupListClear
#include "bdc.h"

static void CorePrioChainFree(CorePrioList *list, CorePrioNode **chain)
{
    CorePrioNode *node;
    CorePrioNode *next;

    for (node = SndEmitterGroupNodeGetNext(*chain); node != NULL; node = next) {
        next = SndEmitterGroupNodeGetNext(node);
        if (list->pool != NULL && MemPoolFree(list->pool, node)) {
            SndEmitterGroupNodeDelete(node, 2);
            node = NULL;
        }
        if (node != NULL) {
            SndEmitterGroupNodeDelete(node, 3);
        }
    }
    SndEmitterGroupNodeSetNext(*chain, NULL);
}

/* Empties both chains of the emitter-group list: walks every node after each sentinel and returns
   it to the node pool (`MemPoolFree`, then `SndEmitterGroupNodeDelete` with flags 2) or frees it
   on the heap (flags 3), and sets both sentinels' `next` to NULL. The payloads (emitter lists) are
   not touched. */
void SndEmitterGroupListClear(CorePrioList *list)
{
    CorePrioChainFree(list, &list->active);
    CorePrioChainFree(list, &list->pending);
}
