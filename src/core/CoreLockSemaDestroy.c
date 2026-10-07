// bdc 0x089bb90c CoreLockSemaDestroy
#include "bdc.h"

/* `CoreLock` Sema backend `destroy`: deletes the lock's kernel semaphore. */
void CoreLockSemaDestroy(CoreLock *lock)
{
    sceKernelDeleteSema(lock->id);
}
