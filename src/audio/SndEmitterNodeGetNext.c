// bdc 0x08a2e744 SndEmitterNodeGetNext
#include "bdc.h"

/* Returns the `next` pointer (`+0xc`) of an emitter-list node; the list walkers follow it from the
   sentinel head. */

CorePrioNode *SndEmitterNodeGetNext(CorePrioNode *node)

{
  return node->next;
}

