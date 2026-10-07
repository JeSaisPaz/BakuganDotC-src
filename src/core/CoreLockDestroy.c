// bdc 0x089bb628 CoreLockDestroy
#include "bdc.h"

/* Destructor of `CoreLock` (GCC 2.x `__in_chrg`-style): if `lock` is non-NULL, calls the
   backend's `destroy` member from `g_coreLockDestroyFns` (deleting the kernel object), clears
   `name`, sets `id = -1`, `lockCount = recursion = 0`, and when `flags & 1` frees the object with
   `MemFree` inside `MemLock`/`MemUnlock` (the deleting-destructor path). */
void CoreLockDestroy(CoreLock *lock, u32 flags)
{
    const MemberFnPtr *member;
    u8 *self;
    void *fn;

    if (lock == NULL) {
        return;
    }
    member = &g_coreLockDestroyFns[lock->type];
    self = (u8 *)lock + member->delta;
    fn = member->pfn;

    /* GCC 2.x pointer-to-member: index != 0 is a virtual slot, pfn then holds the vptr offset */
    if (member->index != 0) {
        const VtblEntry *vtbl = *(const VtblEntry **)(self + (intptr_t)member->pfn);
        const VtblEntry *entry = &vtbl[member->index];

        fn = entry->fn;
        self += entry->delta;
    }
    ((void (*)(void *))fn)(self);

    lock->name = NULL;
    lock->id = -1;
    lock->lockCount = 0;
    lock->recursion = 0;
    if (flags & 1) {
        MemLock();
        MemFree(lock, NULL, 0);
        MemUnlock();
    }
}
