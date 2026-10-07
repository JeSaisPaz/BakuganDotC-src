// bdc 0x08a30ef8 CorePrioNodeSetPriority
#include "bdc.h"

/* Sets the node's sort priority (`+0x4`). */
void CorePrioNodeSetPriority(CorePrioNode *node, s32 priority)
{
    node->priority = priority;
}
