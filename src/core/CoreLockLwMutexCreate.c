// bdc 0x089bba80 CoreLockLwMutexCreate
#include "bdc.h"

/* `CoreLock` LwMutex backend `create` (`CORE_LOCK_LWMUTEX`):
   `sceKernelCreateLwMutex(&lock->workarea, lock->name, 0x300, 0, NULL)`; `lock->lwMutex` points at
   the workarea on success, NULL otherwise. */
void CoreLockLwMutexCreate(CoreLock *lock)
{
    SceLwMutexWorkarea *workarea = &lock->workarea;

    lock->lwMutex = workarea;
    if (workarea != NULL) {
        if (sceKernelCreateLwMutex(workarea, lock->name, 0x300, 0, NULL) != 0) {
            lock->lwMutex = NULL;
        }
        lock->lockCount = 0;
    }
}
