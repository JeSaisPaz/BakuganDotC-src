// bdc 0x089bb888 CoreLockMutexRelease
#include "bdc.h"

/* `CoreLock` Mutex backend `release`: `sceKernelUnlockMutex(lock->id, 1)` and, on success,
   `lock->lockCount--`. */
void CoreLockMutexRelease(CoreLock *lock)
{
    if (lock->id != 0 && sceKernelUnlockMutex(lock->id, 1) == 0) {
        lock->lockCount = lock->lockCount - 1;
    }
}
