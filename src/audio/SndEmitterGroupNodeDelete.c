// bdc 0x08a303b4 SndEmitterGroupNodeDelete
#include "bdc.h"

/* Node 'operator delete': does nothing for NULL or when bit 0 of `flags` is clear (flags 2 =
   destruct only, used for nodes that live in the pool); with bit 0 set it frees the node on the
   heap (`MemFree` under `MemLock`). */
void SndEmitterGroupNodeDelete(CorePrioNode *node, u32 flags)
{
    if (node != NULL && (flags & 1) != 0) {
        MemLock();
        MemFree(node, NULL, 0);
        MemUnlock();
    }
}
