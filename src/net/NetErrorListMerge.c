// bdc 0x08a31d1c NetErrorListMerge
#include "bdc.h"

/* Maintenance pass of the net-error list (counterpart of `CorePrioListMerge`), run before walking it: frees removed nodes from both chains (pool nodes go
   back to the pool) and merges the sorted *pending* chain into the sorted *active* chain (a stable
   two-pointer merge on node priority), then `NetErrorListRewind`s the cursor and empties the
   pending chain. Returns the number of active links visited plus pending nodes moved. */
s32 NetErrorListMerge(CorePrioList *list)
{
    CorePrioNode *prev = list->active;
    CorePrioNode *walk;
    CorePrioNode *node;
    CorePrioNode *incoming;
    s32 count = 0;
    bool unlinked;

    NetErrorNodeGetNext(list->pending);

    /* Drop removed nodes from the pending chain. */
    for (walk = list->pending; walk != NULL; walk = NetErrorNodeGetNext(walk)) {
        node = NetErrorNodeGetNext(walk);
        if (node != NULL && NetErrorNodeIsRemoved(node)) {
            NetErrorNodeSetNext(walk, NetErrorNodeGetNext(node));
            if (list->pool != NULL && MemPoolFree(list->pool, node)) {
                NetErrorNodeDelete(node, 2);
                node = NULL;
            }
            if (node != NULL) {
                NetErrorNodeDelete(node, 3);
            }
        }
    }

    /* Merge the pending chain into the active chain, dropping removed active nodes. */
    incoming = NetErrorNodeGetNext(list->pending);
    while (prev != NULL) {
        node = NetErrorNodeGetNext(prev);
        unlinked = false;
        if (node == NULL) {
            if (incoming != NULL) {
                NetErrorNodeSetNext(prev, incoming);
            }
        } else {
            count++;
            if (NetErrorNodeIsRemoved(node)) {
                NetErrorNodeSetNext(prev, NetErrorNodeGetNext(node));
                if (list->pool != NULL && MemPoolFree(list->pool, node)) {
                    NetErrorNodeDelete(node, 2);
                    node = NULL;
                }
                if (node != NULL) {
                    NetErrorNodeDelete(node, 3);
                }
                node = NetErrorNodeGetNext(prev);
                unlinked = true;
            }
            if (node != NULL) {
                while (incoming != NULL &&
                       NetErrorNodeGetPriority(incoming) < NetErrorNodeGetPriority(node)) {
                    count++;
                    NetErrorNodeSetNext(prev, incoming);
                    incoming = NetErrorNodeGetNext(incoming);
                    NetErrorNodeSetNext(NetErrorNodeGetNext(prev), node);
                    prev = NetErrorNodeGetNext(prev);
                }
            }
        }
        if (!unlinked) {
            prev = node;
        }
    }
    NetErrorListRewind(list);
    NetErrorNodeSetNext(list->pending, NULL);
    return count;
}
