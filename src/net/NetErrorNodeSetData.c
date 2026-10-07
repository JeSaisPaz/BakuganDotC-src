// bdc 0x08a32140 NetErrorNodeSetData
#include "bdc.h"

/* Stores `data` in a net-error list node and sets its state to 1 (in use). Called by
   `NetErrorListAdd`. */
void NetErrorNodeSetData(CorePrioNode *node, void *data)
{
    node->data = data;
    node->state = 1;
}
