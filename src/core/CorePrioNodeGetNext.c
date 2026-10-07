// bdc 0x08a30f10 CorePrioNodeGetNext
#include "bdc.h"

/* Returns the node's `next` link. */
CorePrioNode *CorePrioNodeGetNext(CorePrioNode *node)
{
    return node->next;
}
