// bdc 0x08a2e318 SndEmitterListFlush
#include "bdc.h"

/* Brings the list up to date: deletes nodes flagged removed from both chains, then merges the
   pending chain into the live chain in ascending priority order, empties the pending chain and
   rewinds the cursor (`SndEmitterListRewind`). Returns a count: every node met while walking
   the live chain (removed ones included, counted before they are deleted) plus every pending node
   merged in front of one of them; a pending tail appended at the end of the live chain is not
   counted. `SndEmitterUpdateAll` calls it before each iteration so that emitters added or removed during
   the previous frame take effect. */
s32 SndEmitterListFlush(CorePrioList *list)
{
    CorePrioNode *prev = list->active;
    CorePrioNode *walk;
    CorePrioNode *node;
    CorePrioNode *incoming;
    s32 count = 0;
    bool unlinked;

    SndEmitterNodeGetNext(list->pending);

    for (walk = list->pending; walk != NULL; walk = SndEmitterNodeGetNext(walk)) {
        node = SndEmitterNodeGetNext(walk);
        if (node != NULL && SndEmitterNodeIsRemoved(node)) {
            SndEmitterNodeSetNext(walk, SndEmitterNodeGetNext(node));
            if (list->pool != NULL && MemPoolFree(list->pool, node)) {
                SndEmitterNodeFree(node, 2);
                node = NULL;
            }
            if (node != NULL) {
                SndEmitterNodeFree(node, 3);
            }
        }
    }

    incoming = SndEmitterNodeGetNext(list->pending);
    while (prev != NULL) {
        node = SndEmitterNodeGetNext(prev);
        unlinked = false;
        if (node == NULL) {
            if (incoming != NULL) {
                SndEmitterNodeSetNext(prev, incoming);
            }
        } else {
            count++;
            if (SndEmitterNodeIsRemoved(node)) {
                SndEmitterNodeSetNext(prev, SndEmitterNodeGetNext(node));
                if (list->pool != NULL && MemPoolFree(list->pool, node)) {
                    SndEmitterNodeFree(node, 2);
                    node = NULL;
                }
                if (node != NULL) {
                    SndEmitterNodeFree(node, 3);
                }
                node = SndEmitterNodeGetNext(prev);
                unlinked = true;
            }
            if (node != NULL) {
                while (incoming != NULL &&
                       SndEmitterNodeGetPriority(incoming) < SndEmitterNodeGetPriority(node)) {
                    count++;
                    SndEmitterNodeSetNext(prev, incoming);
                    incoming = SndEmitterNodeGetNext(incoming);
                    SndEmitterNodeSetNext(SndEmitterNodeGetNext(prev), node);
                    prev = SndEmitterNodeGetNext(prev);
                }
            }
        }
        if (!unlinked) {
            prev = node;
        }
    }
    SndEmitterListRewind(list);
    SndEmitterNodeSetNext(list->pending, NULL);
    return count;
}
