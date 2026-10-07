// bdc 0x08a2e06c SndEmitterListLinkSorted
#include "bdc.h"

/* Links `node` into a chain in ascending `priority` order: walks from the sentinel `start` and
   inserts in front of the first node with a strictly greater priority (after nodes of equal
   priority), or at the end. Called by `SndEmitterListInsert` with the pending sentinel; the first
   argument (the list) is unused. */
void SndEmitterListLinkSorted(CorePrioList *list, CorePrioNode *start, CorePrioNode *node)
{
    CorePrioNode *prev;
    CorePrioNode *next;

    (void)list;
    for (prev = start; prev != NULL; prev = next) {
        next = SndEmitterNodeGetNext(prev);
        if (next == NULL) {
            SndEmitterNodeSetNext(prev, node);
            SndEmitterNodeSetNext(node, NULL);
            return;
        }
        if (SndEmitterNodeGetPriority(node) < SndEmitterNodeGetPriority(next)) {
            SndEmitterNodeSetNext(prev, node);
            SndEmitterNodeSetNext(node, next);
            return;
        }
    }
}
