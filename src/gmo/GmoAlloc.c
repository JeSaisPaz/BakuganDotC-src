// bdc 0x089da8cc GmoAlloc
#include "bdc.h"

/* Allocator registered for GMO data (see `GmoSystemInit`): carves `size` bytes (rounded up to 16)
   from the bump region `g_gmoBumpBase` (`g_gmoBumpSize` bytes, current offset stored through
   `g_gmoBumpUsedPtr`) while it has room, otherwise allocates from the low end of the game heap
   (`MemAlloc` under `MemLock`). */

void *GmoAlloc(u32 size)
{
    bool fromLow;
    void *ptr;
    u32 used;

    if (g_gmoBumpBase != NULL) {
        used = *g_gmoBumpUsedPtr;
        if (size < g_gmoBumpSize - used) {
            ptr = (u8 *)g_gmoBumpBase + used;
            *g_gmoBumpUsedPtr = used + ((size + 0xf) & ~0xf);
            return ptr;
        }
    }
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    ptr = MemAlloc(size, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    return ptr;
}
