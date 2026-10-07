// bdc 0x08a31aa0 NetErrorListRemove
#include "bdc.h"

/* Removes the first live entry whose payload equals `data`: searches the active chain, then the
   pending chain, and flags the node removed (`NetErrorNodeMarkRemoved`) without unlinking it; the
   next `NetErrorListMerge` frees it. Returns 1 if found, else 0. Counterpart of
   `CorePrioListRemove`. */

/* Marks the first live node after sentinel `node` whose payload is `data`; returns whether found. */
static inline bool MarkInChain(CorePrioNode *node, void *data)
{
    while (node != NULL) {
        node = NetErrorNodeGetNext(node);
        if (node != NULL && NetErrorNodeGetData(node) == data && !NetErrorNodeIsRemoved(node)) {
            NetErrorNodeMarkRemoved(node);
            return true;
        }
    }
    return false;
}

bool NetErrorListRemove(CorePrioList *list, void *data)
{
    if (MarkInChain(list->active, data)) {
        return true;
    }
    return MarkInChain(list->pending, data);
}
