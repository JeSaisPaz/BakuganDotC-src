// bdc 0x089d85b8 MemDeleteMutex
#include "bdc.h"

/* Releases every outstanding recursion level of `g_memMutex` (`MemUnlock` while
   `g_memLockDepth` > 0) and deletes it with `sceKernelDeleteLwMutex`. Called from
   `MemShutdown`. */
void MemDeleteMutex(void)
{
    while (g_memLockDepth > 0) {
        MemUnlock();
    }
    sceKernelDeleteLwMutex(&g_memMutex);
}
