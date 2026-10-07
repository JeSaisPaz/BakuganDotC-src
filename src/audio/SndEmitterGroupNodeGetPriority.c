// bdc 0x08a3040c SndEmitterGroupNodeGetPriority
#include "bdc.h"

/* Returns the sort key (`+4`) of a node of the emitter-group list; `SndEmitterGroupListFlush`
   compares it to merge the pending chain in ascending order. */

s32 SndEmitterGroupNodeGetPriority(CorePrioNode *node)

{
  return node->priority;
}

