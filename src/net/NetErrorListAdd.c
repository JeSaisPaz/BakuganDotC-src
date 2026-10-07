// bdc 0x08a3188c NetErrorListAdd
#include "bdc.h"

/* Adds `data` with `priority` to the net-error list: takes a 0x10-byte node from the list's pool
   (`+0x10`) or, failing that, from the low end of the game heap, initialises it
   (`NetErrorNodeInit`), sets payload and priority, and inserts it into the pending chain in
   priority order (`NetErrorListInsertSorted`). Returns the node. If both allocations fail the
   NULL node is still passed on (no check). Counterpart of `CorePrioListAdd`. */

CorePrioNode *NetErrorListAdd(CorePrioList *list, void *data, s32 priority)
{
    CorePrioNode *node = NULL;
    CorePrioNode *heapNode;
    bool fromLow;

    if (list->pool != NULL) {
        node = MemPoolAlloc(list->pool);
    }
    if (node != NULL) {
        NetErrorNodeInit(node);
    } else {
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        heapNode = MemAlloc(0x10, NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (heapNode != NULL) {
            NetErrorNodeInit(heapNode);
        }
        node = heapNode;
    }
    NetErrorNodeSetData(node, data);
    NetErrorNodeSetPriority(node, priority);
    NetErrorListInsertSorted(list, list->pending, node);
    return node;
}
