// bdc 0x089bbb30 CoreLockLwMutexAcquire
#include "bdc.h"

/* `CoreLock` LwMutex backend `acquire`: when the lock has a LwMutex work area, calls
   `sceKernelLockLwMutexCB(lock->lwMutex, 1, NULL)` and then increments `lock->lockCount`
   (the call's result is not checked). */
void CoreLockLwMutexAcquire(CoreLock *lock)
{
    if (lock->lwMutex != NULL) {
        sceKernelLockLwMutexCB((SceLwMutex *)lock->lwMutex, 1, NULL);
        lock->lockCount = lock->lockCount + 1;
    }
}
