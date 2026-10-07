// bdc 0x089bb59c CoreLockInit
#include "bdc.h"

/* Constructor of `CoreLock`: stores `name` (+0x10) and `type` (+0x00), resets `id = -1`,
   `lockCount = recursion = 0`, then calls the backend's `create` member from
   `g_coreLockCreateFns` to create the kernel object. Returns `lock`. */
CoreLock *CoreLockInit(CoreLock *lock, char *name, CoreLockType type)
{
    lock->name = name;
    lock->type = type;
    lock->id = -1;
    lock->lockCount = 0;
    lock->recursion = 0;
    {
    const MemberFnPtr *member = &g_coreLockCreateFns[type];
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
    return lock;
}
