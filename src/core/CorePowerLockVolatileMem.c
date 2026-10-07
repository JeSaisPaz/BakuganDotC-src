// bdc 0x089bd0d8 CorePowerLockVolatileMem
#include "bdc.h"

/* Acquires the PSP volatile-memory region (`sceKernelVolatileMemTryLock(0, &base, &size)`) for the
   power service and makes it usable as a second heap. Returns 1 when the region is held after the
   call (including when it already was), 0 when the lock attempt failed. On first success it
   remembers the base in `g_coreVolatileBase`, allocates a 0x1c-byte `MemMng2` object from the
   low heap, builds it with `Mem2Init``(heap, base, size, 0x20, 0x40, 1)` and stores it in
   `CorePowerMgr``.volatileHeap` (NULL when the allocation fails); on later successes it only
   re-seeds that heap with `Mem2Reset`. Records `volatileBase`, `volatileSize` and sets
   `volatileLocked`. */
int CorePowerLockVolatileMem(CorePower *power)
{
    bool fromLow;
    void *heap;
    void *base;
    s32 size;

    if (power->volatileLocked) {
        return 1;
    }
    base = NULL;
    size = 0;
    if (sceKernelVolatileMemTryLock(0, &base, &size) != 0) {
        return 0;
    }
    if (g_corePowerMgr->volatileHeap == NULL) {
        if (g_coreVolatileBase == NULL) {
            g_coreVolatileBase = base;
        }
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        heap = MemAlloc(0x1c, NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (heap != NULL) {
            Mem2Init(heap, base, size, 0x20, 0x40, true);
        }
        g_corePowerMgr->volatileHeap = heap;
    } else {
        Mem2Reset(g_corePowerMgr->volatileHeap, base, size);
    }
    power->volatileSize = size;
    power->volatileBase = base;
    power->volatileLocked = 1;
    return 1;
}
