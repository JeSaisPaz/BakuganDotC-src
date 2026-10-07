// bdc 0x08a2e72c SndEmitterNodeSetPriority
#include "bdc.h"

/* Sets the sort priority (`+4`) of an emitter-list node; lower values sort first. */

void SndEmitterNodeSetPriority(CorePrioNode *node, s32 priority)

{
  node->priority = priority;
  return;
}

