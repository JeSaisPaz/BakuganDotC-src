// bdc 0x08a2e758 SndEmitterNodeIsRemoved
#include "bdc.h"

/* Returns whether an emitter-list node is flagged removed (`state == 2`). */
bool SndEmitterNodeIsRemoved(CorePrioNode *node)
{
    return node->state == 2;
}
