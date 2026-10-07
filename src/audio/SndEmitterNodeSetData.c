// bdc 0x08a2e714 SndEmitterNodeSetData
#include "bdc.h"

/* Stores `data` in an emitter-list node and marks it as holding data (`state = 1`). */
void SndEmitterNodeSetData(CorePrioNode *node, void *data)
{
    node->data = data;
    node->state = 1;
}
