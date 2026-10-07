// bdc 0x08a30ee0 CorePrioNodeSetData
#include "bdc.h"

/* Stores `data` in the node and marks it as in use (`state = 1`). */
void CorePrioNodeSetData(CorePrioNode *node, void *data)
{
    node->data = data;
    node->state = 1;
}
