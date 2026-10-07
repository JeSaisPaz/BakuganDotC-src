// bdc 0x089bbb70 CoreLockLwMutexRelease
#include "bdc.h"

/* `CoreLock` LwMutex backend `release`: when the work area exists,
   `sceKernelUnlockLwMutex(lock->lwMutex, 1)` then `lock->lockCount--` (the unlock result is
   ignored). */
void CoreLockLwMutexRelease(CoreLock *lock)
{
    if (lock->lwMutex != NULL) {
        sceKernelUnlockLwMutex((SceLwMutex *)lock->lwMutex, 1);
        lock->lockCount--;
    }
}
