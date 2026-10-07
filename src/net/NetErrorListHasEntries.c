// bdc 0x08a31f6c NetErrorListHasEntries
#include "bdc.h"

/* Returns whether the net-error list has anything to look at: the cursor is set, or either chain
   has a node after its sentinel. Counterpart of `CorePrioListHasEntries`. */

bool NetErrorListHasEntries(CorePrioList *list)

{
  if (list->cursor != (CorePrioNode *)0x0) {
    return true;
  }
  if (NetErrorNodeGetNext(list->active) != (CorePrioNode *)0x0) {
    return true;
  }
  return NetErrorNodeGetNext(list->pending) != (CorePrioNode *)0x0;
}
