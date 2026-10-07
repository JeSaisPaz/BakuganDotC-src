// bdc 0x08a30d20 CorePrioListHasEntries
#include "bdc.h"

/* Returns whether the list has anything to look at: the cursor is set, or either chain has a node
   after its sentinel. */
bool CorePrioListHasEntries(CorePrioList *list)
{
    if (list->cursor != NULL) {
        return true;
    }
    if (CorePrioNodeGetNext(list->active) != NULL) {
        return true;
    }
    if (CorePrioNodeGetNext(list->pending) != NULL) {
        return true;
    }
    return false;
}
