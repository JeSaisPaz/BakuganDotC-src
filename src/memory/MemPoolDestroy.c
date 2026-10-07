// bdc 0x089d75b8 MemPoolDestroy
#include "bdc.h"

/* Destructor of a `MemPool`: frees the bitmap and the slab (each under `MemLock`) and clears
   the pointers; when bit 0 of `flags` is set it also frees the pool header itself (the C++ `delete`
   flag, so callers pass 3 to delete, 2 to only destruct). Does nothing for a NULL pool. */
void MemPoolDestroy(MemPool *pool, u32 flags)
{
    if (pool == NULL) {
        return;
    }
    if (pool->bitmap != NULL) {
        MemLock();
        MemFree(pool->bitmap, NULL, 0);
        MemUnlock();
        pool->bitmap = NULL;
    }
    if (pool->base != NULL) {
        MemLock();
        MemFree(pool->base, NULL, 0);
        MemUnlock();
        pool->base = NULL;
    }
    if ((flags & 1) != 0) {
        MemLock();
        MemFree(pool, NULL, 0);
        MemUnlock();
    }
}
