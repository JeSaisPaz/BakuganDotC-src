// bdc 0x089bbacc CoreLockLwMutexDestroy
#include "bdc.h"

/* `CoreLock` LwMutex backend `destroy`: releases every outstanding hold
   (`CoreLockLwMutexRelease` while `lockCount > 0`), then deletes the LwMutex and clears
   `lwMutex` on success. Does nothing when there is no LwMutex. */
void CoreLockLwMutexDestroy(CoreLock *lock)
{
    if (lock->lwMutex == NULL)
        return;
    while (lock->lockCount > 0)
        CoreLockLwMutexRelease(lock);
    if (sceKernelDeleteLwMutex(lock->lwMutex) == 0)
        lock->lwMutex = NULL;
}
