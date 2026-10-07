// bdc 0x089d8bc0 CoreNodeDtor
#include "bdc.h"

/* Destructor of the base `CoreNode` (`g_coreNodeVtbl` entry 1): resets the vtable pointer to
   the base class, unlinks the node with `CoreNodeUnlink` and, when bit 0 of `flags` is set,
   frees the object under `MemLock`. A NULL `node` is ignored. */
void CoreNodeDtor(CoreNode *node, u32 flags)
{
    if (node == NULL)
        return;
    node->vtable = g_coreNodeVtbl;
    CoreNodeUnlink(node);
    if (flags & 1) {
        MemLock();
        MemFree(node, NULL, 0);
        MemUnlock();
    }
}
