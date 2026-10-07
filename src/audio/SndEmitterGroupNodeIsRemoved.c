// bdc 0x08a30430 SndEmitterGroupNodeIsRemoved
#include "bdc.h"

/* Returns whether a node of the emitter-group list is flagged removed (`state == 2`). */
bool SndEmitterGroupNodeIsRemoved(CorePrioNode *node)
{
    return node->state == 2;
}
