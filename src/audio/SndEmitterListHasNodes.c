// bdc 0x08a2e554 SndEmitterListHasNodes
#include "bdc.h"

/* Returns 1 if the list has anything to visit: the cursor is non-NULL, or the live chain or the
   pending chain has at least one node after its sentinel; else 0. */
bool SndEmitterListHasNodes(CorePrioList *list)
{
    if (list->cursor != NULL) {
        return true;
    }
    if (SndEmitterNodeGetNext(list->active) != NULL) {
        return true;
    }
    if (SndEmitterNodeGetNext(list->pending) != NULL) {
        return true;
    }
    return false;
}
