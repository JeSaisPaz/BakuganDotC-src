// bdc 0x08a32184 NetErrorNodeIsRemoved
#include "bdc.h"

/* Returns whether a net-error list node is in state 2 (removed). Used by `NetErrorListNext`,
   `NetErrorListRemove` and `NetErrorListMerge` to skip or purge removed nodes. */
bool NetErrorNodeIsRemoved(CorePrioNode *node)
{
    return node->state == 2;
}
