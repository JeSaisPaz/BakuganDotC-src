// bdc 0x089cd808 CoreModuleMgrCtor
#include "bdc.h"

/* Constructor of the `COModule` module manager (`CoreModuleMgr`): allocates the 8-slot table
   `g_coreModuleSlots` (0x40 bytes from the low heap, every slot `{id = -1, state = 0}`), clears
   `liveCount` and creates the semaphore `CoreLock` "COModule" (NULL when its allocation fails).
   Returns `mgr`. */
void *CoreModuleMgrCtor(void *mgr)
{
    CoreModuleMgr *self = mgr;
    bool fromLow;
    CoreLock *lock;
    int i;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    g_coreModuleSlots = MemAlloc(8 * sizeof(CoreModuleSlot), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    for (i = 0; i < 8; i++) {
        g_coreModuleSlots[i].id = -1;
        g_coreModuleSlots[i].state = 0;
    }
    self->liveCount = 0;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    lock = MemAlloc(sizeof(CoreLock), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (lock != NULL) {
        CoreLockInit(lock, "COModule", CORE_LOCK_SEMA);
    }
    self->lock = lock;
    return mgr;
}
