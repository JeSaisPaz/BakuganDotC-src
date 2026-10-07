// bdc 0x089bb6e8 CoreLockAcquire
#include "bdc.h"

/* Acquires a `CoreLock`: calls the `acquire` member of its backend,
   `g_coreLockAcquireFns[lock->type]` (`g_coreLockAcquireFns`), which blocks (Mutex/LwMutex/Sema
   via the `*CB` kernel waits, or the thread-id spin lock) and bumps `lock->lockCount`. Always
   paired with `CoreLockRelease` around critical sections; 183 call sites. */
void CoreLockAcquire(CoreLock *lock)
{
    const MemberFnPtr *member = &g_coreLockAcquireFns[lock->type];
    u8 *self = (u8 *)lock + member->delta;
    void *fn = member->pfn;

    /* GCC 2.x pointer-to-member: index != 0 is a virtual slot, pfn then holds the vptr offset */
    if (member->index != 0) {
        const VtblEntry *vtbl = *(const VtblEntry **)(self + (intptr_t)member->pfn);
        const VtblEntry *entry = &vtbl[member->index];

        fn = entry->fn;
        self += entry->delta;
    }
    ((void (*)(void *))fn)(self);
}
