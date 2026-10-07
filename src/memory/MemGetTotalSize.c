// bdc 0x089d7cf0 MemGetTotalSize
#include "bdc.h"

/* Returns `MemMng``.totalSize` of `g_memMng` (0 without a heap); printed as "ヒープ総量"
   (heap total) by `MemAlloc` on failure. */
u32 MemGetTotalSize(void)
{
    if (g_memMng == NULL) {
        return 0;
    }
    return g_memMng->totalSize;
}
