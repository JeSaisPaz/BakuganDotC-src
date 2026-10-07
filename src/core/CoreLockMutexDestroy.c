// bdc 0x089bb800 CoreLockMutexDestroy
#include "bdc.h"

/* `CoreLock` Mutex backend `destroy`: deletes the kernel mutex `lock->id` with
   `sceKernelDeleteMutex` when it exists, and clears `id` if the delete succeeded. */
void CoreLockMutexDestroy(CoreLock *lock)
{
    if (lock->id != 0 && sceKernelDeleteMutex(lock->id) == 0)
        lock->id = 0;
}
