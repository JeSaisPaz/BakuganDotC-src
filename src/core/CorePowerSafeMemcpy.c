// bdc 0x089bcd0c CorePowerSafeMemcpy
#include "bdc.h"

/* `memcpy(dst, src, n)` followed by `sceKernelDcacheWritebackInvalidateRange(dst, n)`, guarded
   against the volatile-memory window: returns 0 and copies nothing until the first volatile lock
   has happened (`g_coreVolatileBase` is NULL). A destination outside `[base, base + 0x400000)` is
   copied straight away (returns 1). A destination inside the window is copied only while the region
   is locked (`volatileLocked`) and `dst` or `src` belongs to the volatile heap (`MemMng2` vtable
   slot 3 range check), checked under the "COPower" lock; otherwise nothing is copied and 0 is
   returned. If `lock` is non-NULL the caller's `CoreLock` is released before and re-acquired
   after taking the power lock, so the power lock is never taken while holding it. */
static int VolatileHeapContains(const MemMng2 *heap, const void *ptr)
{
    const VtblEntry *contains = &heap->vtbl[3];

    return ((int (*)(void *, const void *))contains->fn)((u8 *)heap + contains->delta, ptr);
}

int CorePowerSafeMemcpy(CorePower *power, void *dst, const void *src, u32 n, CoreLock *lock)
{
    uintptr_t base = (uintptr_t)g_coreVolatileBase;
    int copied = 0;
    const MemMng2 *heap;

    if (g_coreVolatileBase == NULL) {
        return 0;
    }
    if (!(base <= (uintptr_t)dst && (uintptr_t)dst < base + 0x400000)) {
        memcpy(dst, src, n);
        sceKernelDcacheWritebackInvalidateRange(dst, n);
        return 1;
    }
    if (lock != NULL) {
        CoreLockRelease(lock);
    }
    CoreLockAcquire(g_corePowerMgr->lock);
    if (power->volatileLocked && g_corePowerMgr->volatileHeap != NULL) {
        heap = g_corePowerMgr->volatileHeap;
        if (VolatileHeapContains(heap, dst) ||
            VolatileHeapContains((const MemMng2 *)g_corePowerMgr->volatileHeap, src)) {
            memcpy(dst, src, n);
            sceKernelDcacheWritebackInvalidateRange(dst, n);
            copied = 1;
        }
    }
    CoreLockRelease(g_corePowerMgr->lock);
    if (lock != NULL) {
        CoreLockAcquire(lock);
    }
    return copied;
}
