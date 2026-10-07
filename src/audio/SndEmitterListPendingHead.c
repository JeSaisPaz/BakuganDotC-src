// bdc 0x08a2df64 SndEmitterListPendingHead
#include "bdc.h"

/* Returns the sentinel node of the pending chain (`list+4`): nodes added by
   `SndEmitterListInsert` wait here until `SndEmitterListFlush` merges them into the live chain.
    */

CorePrioNode *SndEmitterListPendingHead(CorePrioList *list)

{
  return list->pending;
}

