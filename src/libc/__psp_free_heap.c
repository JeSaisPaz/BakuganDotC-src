// bdc 0x089b4a24 __psp_free_heap
#include "bdc.h"

/* Frees the `"UserSbrk"` partition block that backs the newlib heap: if `g_sbrkBlockId` is
   non-zero it calls `sceKernelFreePartitionMemory` on it and clears the id. The heap range
   variables (`g_sbrkHeapBase` etc.) are not reset. */

void __psp_free_heap(void)
{
    if (g_sbrkBlockId != 0) {
        sceKernelFreePartitionMemory(g_sbrkBlockId);
        g_sbrkBlockId = 0;
    }
}
