// bdc 0x08a320d4 NetErrorNodeInit
#include "bdc.h"

/* Initialises a net-error list node: state 0, no data, priority -1, no next link. Returns
   `node`. Called by `NetErrorListInit` and `NetErrorListAdd`. */
CorePrioNode *NetErrorNodeInit(CorePrioNode *node)
{
    node->state = 0;
    node->data = NULL;
    node->priority = -1;
    node->next = NULL;
    return node;
}
