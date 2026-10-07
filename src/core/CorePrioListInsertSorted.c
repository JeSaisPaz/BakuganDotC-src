// bdc 0x08a30838 CorePrioListInsertSorted
#include "bdc.h"

/* Inserts `node` into the chain that starts at sentinel `head`, keeping ascending priority order:
   advances while the next node's priority is `<=` the new node's, so equal keys stay in insertion
   order. A NULL `head` inserts nothing. `list` is unused. */
void CorePrioListInsertSorted(CorePrioList *list, CorePrioNode *head, CorePrioNode *node)
{
    CorePrioNode *prev;
    CorePrioNode *next;

    (void)list;
    for (prev = head; prev != NULL; prev = next) {
        next = CorePrioNodeGetNext(prev);
        if (next == NULL) {
            CorePrioNodeSetNext(prev, node);
            CorePrioNodeSetNext(node, NULL);
            return;
        }
        if (CorePrioNodeGetPriority(node) < CorePrioNodeGetPriority(next)) {
            CorePrioNodeSetNext(prev, node);
            CorePrioNodeSetNext(node, next);
            return;
        }
    }
}
