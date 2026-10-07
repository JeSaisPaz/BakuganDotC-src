// bdc 0x08a30140 SndEmitterGroupListFlush
#include "bdc.h"

/* Brings the emitter-group list up to date (the `Merge` step of the `CorePrioList` class):
   deletes the nodes flagged removed from both chains, merges the pending chain into the live chain
   in ascending priority order (stable for equal priorities), empties the pending chain and rewinds
   the cursor (`SndEmitterGroupListRewind`). Returns the number of nodes it visited while merging.
   Every walker (`SndReleaseAll`, `SndEmitterGroupSelectNearest`) flushes first. */
s32 SndEmitterGroupListFlush(CorePrioList *list)
{
    CorePrioNode *prev = list->active;
    CorePrioNode *walk;
    CorePrioNode *node;
    CorePrioNode *incoming;
    s32 count = 0;
    bool unlinked;

    SndEmitterGroupNodeGetNext(list->pending);

    for (walk = list->pending; walk != NULL; walk = SndEmitterGroupNodeGetNext(walk)) {
        node = SndEmitterGroupNodeGetNext(walk);
        if (node != NULL && SndEmitterGroupNodeIsRemoved(node)) {
            SndEmitterGroupNodeSetNext(walk, SndEmitterGroupNodeGetNext(node));
            if (list->pool != NULL && MemPoolFree(list->pool, node)) {
                SndEmitterGroupNodeDelete(node, 2);
                node = NULL;
            }
            if (node != NULL) {
                SndEmitterGroupNodeDelete(node, 3);
            }
        }
    }

    incoming = SndEmitterGroupNodeGetNext(list->pending);
    while (prev != NULL) {
        node = SndEmitterGroupNodeGetNext(prev);
        unlinked = false;
        if (node == NULL) {
            if (incoming != NULL) {
                SndEmitterGroupNodeSetNext(prev, incoming);
            }
        } else {
            count++;
            if (SndEmitterGroupNodeIsRemoved(node)) {
                SndEmitterGroupNodeSetNext(prev, SndEmitterGroupNodeGetNext(node));
                if (list->pool != NULL && MemPoolFree(list->pool, node)) {
                    SndEmitterGroupNodeDelete(node, 2);
                    node = NULL;
                }
                if (node != NULL) {
                    SndEmitterGroupNodeDelete(node, 3);
                }
                node = SndEmitterGroupNodeGetNext(prev);
                unlinked = true;
            }
            if (node != NULL) {
                while (incoming != NULL &&
                       SndEmitterGroupNodeGetPriority(incoming) < SndEmitterGroupNodeGetPriority(node)) {
                    count++;
                    SndEmitterGroupNodeSetNext(prev, incoming);
                    incoming = SndEmitterGroupNodeGetNext(incoming);
                    SndEmitterGroupNodeSetNext(SndEmitterGroupNodeGetNext(prev), node);
                    prev = SndEmitterGroupNodeGetNext(prev);
                }
            }
        }
        if (!unlinked) {
            prev = node;
        }
    }
    SndEmitterGroupListRewind(list);
    SndEmitterGroupNodeSetNext(list->pending, NULL);
    return count;
}
