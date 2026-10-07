// bdc 0x089d8b64 CoreNodeCtorMode
#include "bdc.h"

/* Same as `CoreNodeCtor` but the link mode is a parameter: `mode` is passed straight to
   `CoreNodeLink` (0 = append at the end of the anchor's chain, non-zero = insert directly behind
   the anchor). Returns `node`. */
CoreNode *CoreNodeCtorMode(CoreNode *node, CoreNode *anchor, u8 mode)
{
    node->vtable = g_coreNodeVtbl;
    node->group = NULL;
    node->owner = NULL;
    CoreNodeLink(node, anchor, mode);
    node->unk08 = 0;
    g_coreNodeCounter++;
    node->id = g_coreNodeCounter;
    return node;
}
