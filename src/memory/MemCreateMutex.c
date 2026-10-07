// bdc 0x089d857c MemCreateMutex
#include "bdc.h"

/* Creates the heap lock `g_memMutex` (`sceKernelCreateLwMutex(&g_memMutex,
   "FOMemMng_MutexCreate_LwMutexWork", 0x300, 0, NULL)`) and resets `g_memLockDepth` to 0. Called
   at the end of `MemInit`. */
void MemCreateMutex(void)
{
    sceKernelCreateLwMutex(&g_memMutex, "FOMemMng_MutexCreate_LwMutexWork", 0x300, 0, NULL);
    g_memLockDepth = 0;
}
