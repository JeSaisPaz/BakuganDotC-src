// bdc 0x089bb7c0 CoreLockMutexCreate
#include "bdc.h"

/* `CoreLock` Mutex backend `create` (`CORE_LOCK_MUTEX`, entry of `g_coreLockCreateFns`):
   `lock->id = sceKernelCreateMutex(lock->name, 0x300, 0, NULL)`, stored as 0 when the uid is not
   positive. */
void CoreLockMutexCreate(CoreLock *lock)
{
    s32 uid;

    uid = sceKernelCreateMutex(lock->name, 0x300, 0, NULL);
    lock->id = uid;
    if (uid < 1) {
        lock->id = 0;
    }
}
