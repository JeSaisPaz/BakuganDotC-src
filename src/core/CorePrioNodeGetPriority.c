// bdc 0x08a30f00 CorePrioNodeGetPriority
#include "bdc.h"

/* Returns the node's sort priority. */
s32 CorePrioNodeGetPriority(CorePrioNode *node)
{
    return node->priority;
}
