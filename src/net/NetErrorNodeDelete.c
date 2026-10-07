// bdc 0x08a320f0 NetErrorNodeDelete
#include "bdc.h"

/* Deleting-destructor thunk of a net-error list node: frees `node` under `MemLock` when bit 0 of
   `flags` is set. Byte-identical to `CorePrioNodeDelete` / `SysUtilCellDelete`. */
void NetErrorNodeDelete(CorePrioNode *node, u32 flags)
{
    if (node != NULL && (flags & 1) != 0) {
        MemLock();
        MemFree(node, NULL, 0);
        MemUnlock();
    }
}
