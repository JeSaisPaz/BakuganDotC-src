// bdc 0x08a30e74 CorePrioNodeInit
#include "bdc.h"

/* Initialises a `CorePrioNode`: state 0, priority -1, data NULL, next NULL. Returns `node`. */
CorePrioNode *CorePrioNodeInit(CorePrioNode *node)
{
    node->state = 0;
    node->data = NULL;
    node->priority = -1;
    node->next = NULL;
    return node;
}
