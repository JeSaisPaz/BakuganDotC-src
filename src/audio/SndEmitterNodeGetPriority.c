// bdc 0x08a2e734 SndEmitterNodeGetPriority
#include "bdc.h"

/* Returns the sort priority (`+4`) of an emitter-list node. */

s32 SndEmitterNodeGetPriority(CorePrioNode *node)

{
  return node->priority;
}

