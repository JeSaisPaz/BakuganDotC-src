// bdc 0x089d8fc0 CoreNodeRegistryAdd
#include "bdc.h"

/* Registers `node` in the global manager registry: on first use it allocates the root
   (`CoreNodeCtor` on a 0x24-byte block taken from the low heap) into `g_coreNodeRoot` (NULL
   if the allocation failed), then links `node` directly behind the root with `CoreNodeLink`
   (mode 1). A NULL `node` only creates the root. */
void CoreNodeRegistryAdd(CoreNode *node)
{
    if (g_coreNodeRoot == NULL) {
        CoreNode *root;
        bool fromLow;

        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        root = MemAlloc(0x24, NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (root != NULL) {
            CoreNodeCtor(root, NULL);
        }
        g_coreNodeRoot = root;
    }
    if (node != NULL) {
        CoreNodeLink(node, g_coreNodeRoot, 1);
    }
}
