// bdc 0x089d8b08 CoreNodeCtor
#include "bdc.h"

/* Constructor of the base `CoreNode`: installs `g_coreNodeVtbl`, clears `owner` and `group`,
   links the node behind `anchor` with `CoreNodeLink` in insert-after mode (a NULL `anchor`
   leaves the node unlinked), clears `unk08` and stores the next id (`++``g_coreNodeCounter`).
   Returns `node`. */
CoreNode *CoreNodeCtor(CoreNode *node, CoreNode *anchor)
{
    node->vtable = g_coreNodeVtbl;
    node->group = NULL;
    node->owner = NULL;
    CoreNodeLink(node, anchor, 1);
    node->unk08 = 0;
    g_coreNodeCounter++;
    node->id = g_coreNodeCounter;
    return node;
}
