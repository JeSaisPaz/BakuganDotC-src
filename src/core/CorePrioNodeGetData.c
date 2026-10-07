// bdc 0x08a30ef0 CorePrioNodeGetData
#include "bdc.h"

/* Returns the node's payload pointer (`+0x8`). */
void *CorePrioNodeGetData(CorePrioNode *node)
{
    return node->data;
}
