// bdc 0x089d7d28 MemGetFreeSize
#include "bdc.h"

/* Returns `MemMng``.freeSize` of `g_memMng` (0 without a heap); printed as "メモリ残量"
   (memory remaining) by `MemAlloc` on failure. */
u32 MemGetFreeSize(void)
{
    if (g_memMng == NULL) {
        return 0;
    }
    return g_memMng->freeSize;
}
