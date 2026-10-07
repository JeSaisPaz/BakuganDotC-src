// bdc 0x08a308e4 CorePrioListRemove
#include "bdc.h"

/* Removes the first live entry whose payload equals `data`: searches the active chain, then the
   pending chain, and flags the node removed (`CorePrioNodeMarkRemoved`) without unlinking it (the
   next `CorePrioListMerge` frees it). Returns 1 if found, else 0. */

/* Marks the first live node after sentinel `head` whose payload is `data`; returns whether found. */
static inline bool MarkInChain(CorePrioNode *node, void *data)
{
    while (node != NULL) {
        node = CorePrioNodeGetNext(node);
        if (node != NULL && CorePrioNodeGetData(node) == data && !CorePrioNodeIsRemoved(node)) {
            CorePrioNodeMarkRemoved(node);
            return true;
        }
    }
    return false;
}

bool CorePrioListRemove(CorePrioList *list, void *data)
{
    if (MarkInChain(list->active, data)) {
        return true;
    }
    return MarkInChain(list->pending, data);
}
