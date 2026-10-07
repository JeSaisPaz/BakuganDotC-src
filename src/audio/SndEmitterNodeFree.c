// bdc 0x08a2e6c4 SndEmitterNodeFree
#include "bdc.h"

/* Destructor-style release of an emitter-list node: if `node` is non-NULL and bit 0 of `flags` is
   set, frees it to the heap under `MemLock`. Callers pass 3 for heap-allocated nodes (free) and 2
   for pooled nodes (only destruct, the `MemPool` owns the memory). */
void SndEmitterNodeFree(CorePrioNode *node, u32 flags)
{
    if (node != NULL && (flags & 1) != 0) {
        MemLock();
        MemFree(node, NULL, 0);
        MemUnlock();
    }
}
