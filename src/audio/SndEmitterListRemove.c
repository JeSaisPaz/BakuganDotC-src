// bdc 0x08a2e118 SndEmitterListRemove
#include "bdc.h"

/* Removes the entry whose data equals `data`: finds the first not-yet-removed node holding it in
   the live chain, else in the pending chain, and flags it removed (`SndEmitterNodeMarkRemoved`).
   Returns 1 if found, else 0; the node and its memory are reclaimed by the next
   `SndEmitterListFlush`. */

bool SndEmitterListRemove(CorePrioList *list, SndEmitter *data) {
    CorePrioNode *node;
    bool found = false;

    node = list->active;
    while (node != NULL) {
        node = SndEmitterNodeGetNext(node);
        if (node != NULL && SndEmitterNodeGetData(node) == data && !SndEmitterNodeIsRemoved(node)) {
            SndEmitterNodeMarkRemoved(node);
            found = true;
            break;
        }
    }
    if (!found) {
        node = list->pending;
        while (node != NULL) {
            node = SndEmitterNodeGetNext(node);
            if (node != NULL && SndEmitterNodeGetData(node) == data && !SndEmitterNodeIsRemoved(node)) {
                SndEmitterNodeMarkRemoved(node);
                found = true;
                break;
            }
        }
    }
    return found;
}
