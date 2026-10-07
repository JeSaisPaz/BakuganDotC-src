// bdc 0x089bb96c CoreLockSemaRelease
#include "bdc.h"

/* `CoreLock` Sema backend `release`: `sceKernelSignalSema(lock->id, 1)` and, on success,
   `lock->lockCount--`. */
void CoreLockSemaRelease(CoreLock *lock)
{
    if (sceKernelSignalSema(lock->id, 1) == 0) {
        lock->lockCount--;
    }
}
