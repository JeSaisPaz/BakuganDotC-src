// bdc 0x08a30f24 CorePrioNodeIsRemoved
#include "bdc.h"

/* Returns whether the node is flagged removed (`state == 2`). */
bool CorePrioNodeIsRemoved(CorePrioNode *node)
{
    return node->state == 2;
}
