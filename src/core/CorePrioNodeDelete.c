// bdc 0x08a30e90 CorePrioNodeDelete
#include "bdc.h"

/* Deleting-destructor thunk for a `CorePrioNode`: when bit 0 of `flags` is set, frees `node`
   under `MemLock`. Flag 2 callers (pool nodes) only run the no-op destructor part. */
void CorePrioNodeDelete(CorePrioNode *node, u32 flags)
{
    if (node != NULL && (flags & 1) != 0) {
        MemLock();
        MemFree(node, NULL, 0);
        MemUnlock();
    }
}
