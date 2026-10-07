// bdc 0x089bb928 CoreLockSemaAcquire
#include "bdc.h"

/* `CoreLock` Sema backend `acquire`: `sceKernelWaitSemaCB(lock->id, 1, NULL)` and, on success,
   `lock->lockCount++`. */
void CoreLockSemaAcquire(CoreLock *lock)
{
    if (sceKernelWaitSemaCB(lock->id, 1, NULL) == 0) {
        lock->lockCount++;
    }
}
