// bdc 0x08a3044c CorePrioListInit
#include "bdc.h"

/* Constructor of a `CorePrioList`: builds the node pool (`CorePrioListCreatePool`, `poolCount`
   nodes), then allocates and initialises the two sentinel nodes `active` and `pending` — from the
   pool when one exists, otherwise 16 bytes from the low heap (`CorePrioNodeInit`; NULL if that
   allocation fails) — and clears `cursor` and `iterating`. Returns `list`. */

/* Initialises a pool-allocated sentinel, or allocates one from the low heap when `node` is NULL. */
static inline CorePrioNode *SentinelInit(CorePrioNode *node)
{
    if (node == NULL) {
        bool fromLow;

        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        node = MemAlloc(0x10, NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (node == NULL) {
            return NULL;
        }
    }
    CorePrioNodeInit(node);
    return node;
}

CorePrioList *CorePrioListInit(CorePrioList *list, s32 poolCount)
{
    list->pool = NULL;
    CorePrioListCreatePool(list, poolCount);
    list->active = NULL;
    list->pending = NULL;
    if (list->pool != NULL) {
        list->active = MemPoolAlloc(list->pool);
        list->pending = MemPoolAlloc(list->pool);
    }
    list->active = SentinelInit(list->active);
    list->pending = SentinelInit(list->pending);
    list->cursor = NULL;
    list->iterating = 0;
    return list;
}
