// bdc 0x089d863c MemUnlock
#include "bdc.h"

/* Unlocks the game-heap mutex `g_memMutex` (`sceKernelUnlockLwMutex(&g_memMutex, 1)`) and
   decrements `g_memLockDepth`. Counterpart of `MemLock`. */
void MemUnlock(void)
{
    sceKernelUnlockLwMutex((SceLwMutex *)&g_memMutex, 1);
    g_memLockDepth--;
}
