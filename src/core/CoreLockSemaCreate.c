// bdc 0x089bb8d0 CoreLockSemaCreate
#include "bdc.h"

/* `CoreLock` Sema backend `create` (`CORE_LOCK_SEMA`): `lock->id =
   sceKernelCreateSema(lock->name, 0, 1, 1, NULL)`, a binary semaphore that starts free. */
void CoreLockSemaCreate(CoreLock *lock)
{
    lock->id = sceKernelCreateSema(lock->name, 0, 1, 1, NULL);
}
