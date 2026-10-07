// bdc 0x08a32158 NetErrorNodeSetPriority
#include "bdc.h"

/* Stores the sort priority (`+0x4`) of a net-error list node. Counterpart of
   `CorePrioNodeSetPriority`. */

void NetErrorNodeSetPriority(CorePrioNode *node, s32 priority)

{
  node->priority = priority;
  return;
}

