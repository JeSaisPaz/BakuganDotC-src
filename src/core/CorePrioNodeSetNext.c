// bdc 0x08a30f08 CorePrioNodeSetNext
#include "bdc.h"

/* Sets the node's `next` link. */
void CorePrioNodeSetNext(CorePrioNode *node, CorePrioNode *next)
{
    node->next = next;
}
