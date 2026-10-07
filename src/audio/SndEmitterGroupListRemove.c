// bdc 0x08a2ff40 SndEmitterGroupListRemove
#include "bdc.h"

/* Removes the entry whose payload is `data` (an emitter list) from the emitter-group list: scans
   the live chain and then the pending chain for the first node with that payload that is not
   already removed and flags it removed (`SndEmitterGroupNodeMarkRemoved`, state 2; the node is
   freed by the next `SndEmitterGroupListFlush`). Returns 1 if a node was flagged, 0 if `data` is
   not in the list. */

bool SndEmitterGroupListRemove(CorePrioList *list, CorePrioList *data) {
    CorePrioNode *node;
    bool found = false;

    node = list->active;
    while (node != NULL) {
        node = SndEmitterGroupNodeGetNext(node);
        if (node != NULL && SndEmitterGroupNodeGetData(node) == data && !SndEmitterGroupNodeIsRemoved(node)) {
            SndEmitterGroupNodeMarkRemoved(node);
            found = true;
            break;
        }
    }
    if (!found) {
        node = list->pending;
        while (node != NULL) {
            node = SndEmitterGroupNodeGetNext(node);
            if (node != NULL && SndEmitterGroupNodeGetData(node) == data && !SndEmitterGroupNodeIsRemoved(node)) {
                SndEmitterGroupNodeMarkRemoved(node);
                found = true;
                break;
            }
        }
    }
    return found;
}
