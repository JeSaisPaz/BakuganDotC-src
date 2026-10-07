// bdc 0x089d7d0c MemGetUsedSize
#include "bdc.h"

/* Returns `MemMng``.usedSize` of `g_memMng` (0 without a heap); printed as "確保メモリ"
   (allocated memory) by `MemAlloc` on failure. */
u32 MemGetUsedSize(void)
{
    if (g_memMng == NULL) {
        return 0;
    }
    return g_memMng->usedSize;
}
