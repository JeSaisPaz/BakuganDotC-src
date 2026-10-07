// bdc 0x08a30f18 CorePrioNodeMarkRemoved
#include "bdc.h"

/* Flags the node as removed (`state = 2`). Walkers skip such nodes and `CorePrioListMerge` frees
   them. */
void CorePrioNodeMarkRemoved(CorePrioNode *node)
{
    node->state = 2;
}
