// bdc 0x089bb840 CoreLockMutexAcquire
#include "bdc.h"

/* `CoreLock` Mutex backend `acquire`: `sceKernelLockMutexCB(lock->id, 1, NULL)` and, on success,
   `lock->lockCount++`. Does nothing when `id` is 0. */
void CoreLockMutexAcquire(CoreLock *lock)
{
    if (lock->id != 0 && sceKernelLockMutexCB(lock->id, 1, NULL) == 0) {
        lock->lockCount++;
    }
}
