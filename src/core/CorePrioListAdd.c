// bdc 0x08a30738 CorePrioListAdd
#include "bdc.h"

/* Registers `data` with a priority: takes a node from the pool (`MemPoolAlloc`) or, if there is
   no pool or it is exhausted, 16 bytes from the low heap, initialises it (`CorePrioNodeInit`,
   `CorePrioNodeSetData`, `CorePrioNodeSetPriority`) and inserts it into the *pending* chain
   with `CorePrioListInsertSorted`. It becomes visible to `CorePrioListNext`/head walks after
   the next `CorePrioListMerge`. Returns the node (no NULL check if the heap allocation fails). */
CorePrioNode *CorePrioListAdd(CorePrioList *list, void *data, s32 priority)
{
    bool fromLow;
    CorePrioNode *node;

    node = NULL;
    if (list->pool != NULL) {
        node = MemPoolAlloc(list->pool);
    }
    if (node != NULL) {
        CorePrioNodeInit(node);
    } else {
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        node = MemAlloc(sizeof(CorePrioNode), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (node != NULL) {
            CorePrioNodeInit(node);
        }
    }
    CorePrioNodeSetData(node, data);
    CorePrioNodeSetPriority(node, priority);
    CorePrioListInsertSorted(list, list->pending, node);
    return node;
}
