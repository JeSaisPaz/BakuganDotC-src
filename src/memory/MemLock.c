// bdc 0x089d8604 MemLock
#include "bdc.h"

/* Locks the game-heap mutex `g_memMutex` (`sceKernelLockLwMutexCB(&g_memMutex, 1, NULL)`, recursive)
   and increments `g_memLockDepth`. Callers wrap `MemAlloc`/`MemFree` (which do not lock
   internally) between MemLock and `MemUnlock`. */
void MemLock(void)
{
    sceKernelLockLwMutexCB((SceLwMutex *)&g_memMutex, 1, NULL);
    g_memLockDepth++;
}
