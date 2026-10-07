// bdc 0x089bb750 CoreLockRelease
#include "bdc.h"

/* Releases one hold on a `CoreLock`: calls `g_coreLockReleaseFns[lock->type]`
   (`g_coreLockReleaseFns`) — sceKernelUnlockMutex / sceKernelUnlockLwMutex /
   sceKernelSignalSema or the spin-lock unlock — which decrements `lock->lockCount`. Counterpart
   of `CoreLockAcquire`; 183 call sites (the same set of callers). */
void CoreLockRelease(CoreLock *lock)
{
    const MemberFnPtr *member = &g_coreLockReleaseFns[lock->type];
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
