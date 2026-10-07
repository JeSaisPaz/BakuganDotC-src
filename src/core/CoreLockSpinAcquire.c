// bdc 0x089bb9bc CoreLockSpinAcquire
#include "bdc.h"

/* `CoreLock` Spin backend `acquire` (`CORE_LOCK_SPIN`): recursive spin lock keyed on the thread
   id. Increments `lockCount`; the first holder stores its thread id in `id` and sets `recursion =
   1`, the owner re-entering bumps `recursion`, anyone else undoes the increment and sleeps
   `sceKernelDelayThreadCB(100)` before retrying. */
void CoreLockSpinAcquire(CoreLock *lock)
{
    s32 threadId = sceKernelGetThreadId();
    int count;

    for (;;) {
        count = lock->lockCount;
        lock->lockCount = count + 1;
        if (count + 1 == 1) {
            lock->recursion = 1;
            lock->id = threadId;
            return;
        }
        if (lock->id == threadId) {
            lock->recursion++;
            return;
        }
        lock->lockCount = count;
        sceKernelDelayThreadCB(100);
    }
}
