// bdc 0x08a30ae4 CorePrioListMerge
#include "bdc.h"

/* Maintenance pass before walking the list: frees removed nodes from both chains (pool nodes go
   back to the pool) and merges the sorted *pending* chain into the sorted *active* chain (a stable
   two-pointer merge on node priority), then `CorePrioListRewind`s the cursor and empties the
   pending chain. Returns the number of active links visited plus pending nodes moved. */
s32 CorePrioListMerge(CorePrioList *list)
{
    CorePrioNode *prev = list->active;
    CorePrioNode *walk;
    CorePrioNode *node;
    CorePrioNode *incoming;
    s32 count = 0;
    bool unlinked;

    CorePrioNodeGetNext(list->pending);

    /* Drop removed nodes from the pending chain. */
    for (walk = list->pending; walk != NULL; walk = CorePrioNodeGetNext(walk)) {
        node = CorePrioNodeGetNext(walk);
        if (node != NULL && CorePrioNodeIsRemoved(node)) {
            CorePrioNodeSetNext(walk, CorePrioNodeGetNext(node));
            if (list->pool != NULL && MemPoolFree(list->pool, node)) {
                CorePrioNodeDelete(node, 2);
                node = NULL;
            }
            if (node != NULL) {
                CorePrioNodeDelete(node, 3);
            }
        }
    }

    /* Merge the pending chain into the active chain, dropping removed active nodes. */
    incoming = CorePrioNodeGetNext(list->pending);
    while (prev != NULL) {
        node = CorePrioNodeGetNext(prev);
        unlinked = false;
        if (node == NULL) {
            if (incoming != NULL) {
                CorePrioNodeSetNext(prev, incoming);
            }
        } else {
            count++;
            if (CorePrioNodeIsRemoved(node)) {
                CorePrioNodeSetNext(prev, CorePrioNodeGetNext(node));
                if (list->pool != NULL && MemPoolFree(list->pool, node)) {
                    CorePrioNodeDelete(node, 2);
                    node = NULL;
                }
                if (node != NULL) {
                    CorePrioNodeDelete(node, 3);
                }
                node = CorePrioNodeGetNext(prev);
                unlinked = true;
            }
            if (node != NULL) {
                while (incoming != NULL &&
                       CorePrioNodeGetPriority(incoming) < CorePrioNodeGetPriority(node)) {
                    count++;
                    CorePrioNodeSetNext(prev, incoming);
                    incoming = CorePrioNodeGetNext(incoming);
                    CorePrioNodeSetNext(CorePrioNodeGetNext(prev), node);
                    prev = CorePrioNodeGetNext(prev);
                }
            }
        }
        if (!unlinked) {
            prev = node;
        }
    }
    CorePrioListRewind(list);
    CorePrioNodeSetNext(list->pending, NULL);
    return count;
}
