// bdc 0x08a32160 NetErrorNodeGetPriority
#include "bdc.h"

/* Returns the sort priority (`+0x4`) of a net-error list node. Counterpart of
   `CorePrioNodeGetPriority`. */

s32 NetErrorNodeGetPriority(CorePrioNode *node)

{
  return node->priority;
}

