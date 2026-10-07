// bdc 0x08a2e6a8 SndEmitterNodeInit
#include "bdc.h"

/* Initialises a node of the emitter list: `state = 0`, `priority = -1`, `data = NULL`, `next =
   NULL`. Returns the node. */
CorePrioNode *SndEmitterNodeInit(CorePrioNode *node)
{
    node->state = 0;
    node->data = NULL;
    node->priority = -1;
    node->next = NULL;
    return node;
}
