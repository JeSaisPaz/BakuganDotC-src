// bdc 0x08a3041c SndEmitterGroupNodeGetNext
#include "bdc.h"

/* Returns the link field (`+0xc`) of a node of the emitter-group list: the next node of its chain,
   NULL at the end. The group walkers start at the sentinel returned by `SndEmitterGroupListHead`.
    */

CorePrioNode *SndEmitterGroupNodeGetNext(CorePrioNode *node)

{
  return node->next;
}

