// bdc 0x08a31558 NetErrorListInit
#include "bdc.h"

/* Constructor of the pending-error list of the `CONetError` manager (`NetErrorMgrCtor`, pool count 0); same template as `CorePrioListInit`: builds the node pool (`NetErrorListCreatePool`, `poolCount`
   nodes), then allocates and initialises the two sentinel nodes `active` and `pending` — from the
   pool when one exists, otherwise 16 bytes from the low heap (`NetErrorNodeInit`; NULL if that
   allocation fails) — and clears `cursor` and `iterating`. Returns `list`. */

/* Initialises a pool-allocated sentinel, or allocates one from the low heap when `node` is NULL. */
static inline CorePrioNode *SentinelInit(CorePrioNode *node)
{
    if (node == NULL) {
        bool fromLow;

        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        node = MemAlloc(sizeof(CorePrioNode), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (node == NULL) {
            return NULL;
        }
    }
    NetErrorNodeInit(node);
    return node;
}

CorePrioList *NetErrorListInit(CorePrioList *list, s32 poolCount)
{
    list->pool = NULL;
    NetErrorListCreatePool(list, poolCount);
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
